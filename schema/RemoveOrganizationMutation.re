open Graphql_lwt;
open GraphqlHelpers;

let payload =
  Schema.(
    obj("ErrorPayload", ~fields=_ =>
      [
        field(
          "error", ~typ=string, ~args=Arg.[], ~resolve=(_: Context.t, error) =>
          error
        ),
      ]
    )
  );

let removeOrganization =
  Schema.(
    io_field(
      "removeOrganization",
      ~typ=non_null(payload),
      ~args=Arg.[arg("companyId", ~typ=non_null(int))],
      ~resolve=(info, (), companyId) =>
      switch (info.user) {
      | Some(user) => Lwt.return(Ok(None))
      | None => Lwt.return(Ok(Some(Errors.unauthorized)))
      }
    )
  );
