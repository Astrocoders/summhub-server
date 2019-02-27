open Graphql_lwt;

let payload =
  Schema.(
    obj(
      "AddOrganizationProjectPayload",
      ~fields=(
                _:
                  Graphql_lwt.Schema.typ(
                    Context.t,
                    option(
                      (option(string), option(Organization.Project.t)),
                    ),
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

let addOrganizationProject =
  Schema.(
    io_field(
      "addOrganizationProject",
      ~typ=non_null(payload),
      ~args=Arg.[arg("organizationId", ~typ=non_null(int))],
      ~resolve=(info, (), _input) =>
      switch (info.user) {
      | Some(user) => Lwt.return(Ok((None, None)))
      | None => Lwt.return(Ok((Some("User not authenticated"), None)))
      }
    )
  );
