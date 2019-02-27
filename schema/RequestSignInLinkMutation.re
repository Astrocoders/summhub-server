open Graphql_lwt;

let payload =
  Schema.(
    obj("ErrorPayload", ~fields=_ =>
      [
        field("error", ~typ=string, ~args=Arg.[], ~resolve=(_: Context.t, error) =>
          error
        ),
      ]
    )
  );

let requestSignInLink =
  Schema.(
    io_field(
      "requestSignInLink",
      ~typ=non_null(payload),
      ~args=Arg.[arg("email", ~typ=non_null(string))],
      ~resolve=(_, (), email) =>
      Lwt.return(Ok(None))
    )
  );
