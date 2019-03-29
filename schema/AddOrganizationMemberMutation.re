open Graphql_lwt;
open GraphqlHelpers;

exception Error_on_add_organization_member;

module Mutation = [%graphql
  {|
  mutation AddOrganizationMemberMutation($input: [members_insert_input!]!) {
    insert_members(objects: $input) {
      returning {
        id
        organization_id
        email
        created_at
      }
    }
  }
|}
];

type addOrganizationMemberInput = {
  organizationId: string,
  email: string,
};

let addOrganizationMemberInput =
  Schema.Arg.(
    obj(
      "AddOrganizationMemberInput",
      ~fields=[
        arg("organizationId", ~typ=non_null(guid)),
        arg("email", ~typ=non_null(string)),
      ],
      ~coerce=(organizationId, email) =>
      {organizationId, email}
    )
  );

let payload =
  Schema.(
    obj(
      "AddOrganizationMemberPayload",
      ~fields=(
                _:
                  Graphql_lwt.Schema.typ(
                    Context.t,
                    option((option(string), option(Member.t))),
                  ),
              ) =>
      [
        field(
          "error",
          ~typ=string,
          ~args=Arg.[],
          ~resolve=(_: Context.t, (error, _)) =>
          error
        ),
        field(
          "member",
          ~typ=Member.typ,
          ~args=Arg.[],
          ~resolve=(_: Context.t, (_, member)) =>
          member
        ),
      ]
    )
  );

[@deriving yojson]
type memberInput = {
  [@key "organization_id"]
  organization_id: string,
  [@key "email"]
  email: string,
};

[@deriving yojson]
type variables = {
  [@key "input"]
  input: memberInput,
};

[@deriving yojson]
type organizationId = {
  [@key "organization_id"]
  organization_id: string,
};

let addOrganizationMember =
  Schema.(
    io_field(
      "addOrganizationMember",
      ~typ=non_null(payload),
      ~args=Arg.[arg("input", ~typ=non_null(addOrganizationMemberInput))],
      ~resolve=(info, (), input) =>
      switch (info.user) {
      | Some(user) =>
        /* OCaml object syntax */
        let queryInput = {
          pub email = Some(input.email);
          pub organizationsByorganizationId = None;
          pub created_at = None;
          /* Needed because graphql_ppx defines scalar types to Json by default */
          pub organization_id =
            Some(
              organizationId_to_yojson({
                organization_id: input.organizationId,
              }),
            );
          pub id = None
        };
        let mutation = Mutation.make(~input=[|queryInput|], ());
        let parseResponse = result => {
          mutation#parse(result)#insert_members
          |> (
            members => {
              switch (members) {
              | Some(members) =>
                Array.length(members#returning) > 0
                  ? members#returning[0]
                  : raise(Error_on_add_organization_member)
              | None => raise(Error_on_add_organization_member)
              };
            }
          );
        };
        let mutationVariables =
          variables_to_yojson({
            input: {
              organization_id: input.organizationId,
              email: input.email,
            },
          });
        let%lwt result =
          Client.get(
            mutation#query,
            ~variables=mutationVariables,
            parseResponse,
          );
        switch (result) {
        | Result.Ok(content) =>
          Lwt.return(
            Ok((
              None,
              Some(
                Member.{
                  id:
                    switch (content#id) {
                    | `String(id) => id
                    | _ => ""
                    },
                  email: content#email,
                },
              ),
            )),
          )
        | Result.Error(_error) =>
          Lwt.return(Ok((Some(Errors.somethingWentWrong), None)))
        };
      | None => Lwt.return(Ok((Some(Errors.unauthorized), None)))
      }
    )
  );
