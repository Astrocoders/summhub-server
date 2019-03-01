open Graphql_lwt;
open GraphqlHelpers;

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
          ~typ=Project.resolver,
          ~args=Arg.[],
          ~resolve=(_: Context.t, (_, project)) =>
          project
        ),
      ]
    )
  );

let addOrganizationProject =
  Schema.(
    io_field(
      "addOrganizationProject",
      ~typ=non_null(payload),
      ~args=Arg.[arg("input", ~typ=non_null(addOrganizationProjectInput))],
      ~resolve=(info, (), _input) =>
      switch (info.user) {
      | Some(user) => Lwt.return(Ok((None, None)))
      | None => Lwt.return(Ok((Some(Errors.unauthorized), None)))
      }
    )
  );
