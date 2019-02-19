open Graphql_lwt;

let forgotPasswordPayload =
  Schema.(
    obj(
      "ForgotPasswordPayload",
      ~fields=(_: Graphql_lwt.Schema.typ(Context.t, option(option(string)))) =>
      [
        field("error", ~typ=string, ~args=Arg.[], ~resolve=(_, error) =>
          error
        ),
      ]
    )
  );

let forgotPassword =
  Schema.(
    io_field(
      "forgotPassword",
      ~typ=non_null(forgotPasswordPayload),
      ~args=Arg.[arg("email", ~typ=non_null(string))],
      ~resolve=(_, (), _email) =>
      Lwt.return(Ok(None))
    )
  );
