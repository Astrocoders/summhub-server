open Graphql_lwt;
open GraphqlHelpers;
open Library;

exception Error_on_add_organization_project;

type addOrganizationProjectInput = {organizationId: string};

let addOrganizationProjectInput =
  Schema.Arg.(
    obj(
      "AddOrganizationProjectInput",
      ~fields=[arg("organizationId", ~typ=non_null(guid))],
      ~coerce=organizationId =>
      organizationId
    )
  );

let payload =
  Schema.(
    obj(
      "AddOrganizationProjectPayload",
      ~fields=(
                _:
                  Graphql_lwt.Schema.typ(
                    Context.t,
                    option((option(string), option(Project.t))),
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
          "project",
          ~typ=Project.typ,
          ~args=Arg.[],
          ~resolve=(_: Context.t, (_, project)) =>
          project
        ),
      ]
    )
  );

module Mutation = [%graphql
  {|
  mutation AddOrganizationMemberMutation($input: [projects_insert_input!]!) {
    insert_projects(objects: $input) {
      returning {
        id
        organization_id
        name
        created_at
        webhook
      }
    }
  }
|}
];

[@deriving yojson]
type projectInput = {
  [@key "organization_id"]
  organization_id: string,
  [@key "name"]
  name: string,
  [@key "webhook"]
  webhook: string,
};

[@deriving yojson]
type variables = {
  [@key "input"]
  input: projectInput,
};

let addOrganizationProject =
  Schema.(
    io_field(
      "addOrganizationProject",
      ~typ=non_null(payload),
      ~args=Arg.[arg("input", ~typ=non_null(addOrganizationProjectInput))],
      ~resolve=(info, (), input) =>
      switch (info.user) {
      | Some(user) =>
        /* OCaml object syntax */
        let queryInput = {
          pub organizationsByorganizationId = None;
          pub created_at = None;
          /* Needed because graphql_ppx defines scalar types to Json by default */
          pub organization_id =
            Some(`Assoc([("organization_id", `String(input))]));
          pub webhook = None;
          pub name = Some("New Project");
          pub id = None
        };
        let mutation = Mutation.make(~input=[|queryInput|], ());
        let parseResponse = result => {
          mutation#parse(result)#insert_projects
          |> (
            projects => {
              switch (projects) {
              | Some(projects) =>
                Array.length(projects#returning) > 0
                  ? projects#returning[0]
                  : raise(Error_on_add_organization_project)
              | None => raise(Error_on_add_organization_project)
              };
            }
          );
        };
        let mutationVariables =
          variables_to_yojson({
            input: {
              organization_id: input,
              name: "New Project",
              webhook: "",
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
                Project.{
                  id: Util.JSON.getStringWithDefault("", content#id),
                  name: content#name,
                  webhook: content#webhook,
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
