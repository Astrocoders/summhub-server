open Graphql_lwt;
open GraphqlHelpers;
open Library;

exception Error_on_add_organization;

let payload =
  Schema.(
    obj(
      "CreateOrganizationPayload",
      ~fields=(
                _:
                  Graphql_lwt.Schema.typ(
                    Context.t,
                    option((option(string), option(Organization.t))),
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
          "organization",
          ~typ=Organization.typ,
          ~args=Arg.[],
          ~resolve=(_: Context.t, (_, organization)) =>
          organization
        ),
      ]
    )
  );

module Mutation = [%graphql
  {|
    mutation CreateOrganizationMutation($input: [organizations_insert_input!]!) {
      insert_organizations(objects: $input) {
        returning {
          id
          name
          user_id
          created_at
        }
      }
    }
  |}
];

[@deriving yojson]
type organizationInput = {
  [@key "user_id"]
  user_id: string,
  [@key "name"]
  name: string,
};

[@deriving yojson]
type variables = {
  [@key "input"]
  input: organizationInput,
};

let createOrganization =
  Schema.(
    io_field(
      "createOrganization",
      ~typ=non_null(payload),
      ~args=Arg.[],
      ~resolve=(context, ()) =>
      switch (context.user) {
      | Some(user) =>
        /* OCaml object syntax */
        let queryInput = {
          pub appUsersByuserId = None;
          pub created_at = None;
          pub id = None;
          pub memberssByorganizationId = None;
          pub name = Some("Organization Created");
          pub projectssByorganizationId = None;
          pub user_id = Some(`Assoc([("user_id", `String(user.id))]))
        };
        let mutation = Mutation.make(~input=[|queryInput|], ());
        let parseResponse = result => {
          mutation#parse(result)#insert_organizations
          |> (
            organizations => {
              switch (organizations) {
              | Some(organizations) =>
                Array.length(organizations#returning) > 0
                  ? organizations#returning[0]
                  : raise(Error_on_add_organization)
              | None => raise(Error_on_add_organization)
              };
            }
          );
        };
        let mutationVariables =
          variables_to_yojson({
            input: {
              user_id: user.id,
              name: "Organization Created",
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
                Organization.{
                  id: Util.JSON.getStringWithDefault("", content#id),
                  name: content#name,
                  createdAt: Util.JSON.encodeCreatedAt(content#created_at),
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
