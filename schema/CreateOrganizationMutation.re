open Graphql_lwt;

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
          ~typ=Organization.resolver,
          ~args=Arg.[],
          ~resolve=(_: Context.t, (_, organization)) =>
          organization
        ),
      ]
    )
  );

let createOrganization =
  Schema.(
    io_field(
      "createOrganization",
      ~typ=non_null(payload),
      ~args=Arg.[],
      ~resolve=(info, ()) =>
      switch (info.user) {
      | Some(user) => Lwt.return(Ok((None, None)))
      | None => Lwt.return(Ok((Some("User not authenticated"), None)))
      }
    )
  );
