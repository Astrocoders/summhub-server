open Graphql_lwt;
open GraphqlHelpers;

type updateOrganizationProjectNameInput = {
  projectId: int,
  name: string,
};

let updateOrganizationProjectNameInput =
  Schema.Arg.(
    obj(
      "UpdateOrganizationProjectNameInput",
      ~fields=[
        arg("projectId", ~typ=non_null(int)),
        arg("name", ~typ=non_null(string)),
      ],
      ~coerce=(projectId, name) =>
      {projectId, name}
    )
  );

let payload =
  Schema.(
    obj(
      "UpdateOrganizationProjectNamePayload",
      ~fields=(
                _:
                  Graphql_lwt.Schema.typ(
                    Context.t,
                    option((option(string), option(Organization.Project.t))),
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
          ~typ=Organization.Project.resolver,
          ~args=Arg.[],
          ~resolve=(_: Context.t, (_, project)) =>
          project
        ),
      ]
    )
  );

let updateOrganizationProjectName =
  Schema.(
    io_field(
      "updateOrganizationProjectName",
      ~typ=non_null(payload),
      ~args=Arg.[arg("input", ~typ=non_null(updateOrganizationProjectNameInput))],
      ~resolve=(info, (), _input) =>
      switch (info.user) {
      | Some(user) => Lwt.return(Ok((None, None)))
      | None => Lwt.return(Ok((Some(Errors.unauthorized), None)))
      }
    )
  );
