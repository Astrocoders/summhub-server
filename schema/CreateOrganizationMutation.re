open Graphql_lwt;
open GraphqlHelpers;

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

let createOrganization =
  Schema.(
    io_field(
      "createOrganization",
      ~typ=non_null(payload),
      ~args=Arg.[],
      ~resolve=(context, ()) =>
      switch (context.user) {
      | Some(user) =>
        let%lwt result =
          Organization.insert(context.connection, ~userId=user.id);
        switch (result) {
        | None => Lwt.return(Ok((Some(Errors.somethingWentWrong), None)))
        | Some(organization) => Lwt.return(Ok((None, Some(organization))))
        };
      | None => Lwt.return(Ok((Some(Errors.unauthorized), None)))
      }
    )
  );
