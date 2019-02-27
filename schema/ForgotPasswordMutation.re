open Graphql_lwt;

type forgotPasswordInput = {email: string};

let forgotPasswordInput =
  Schema.Arg.(
    obj(
      "ForgotPasswordInput",
      ~fields=[arg("email", ~typ=non_null(string))],
      ~coerce=email =>
      {email}
    )
  );

let forgotPassword =
  Schema.(
    io_field(
      "forgotPassword",
      ~typ=non_null(GraphqlTypes.errorPayload),
      ~args=Arg.[arg("input", ~typ=non_null(forgotPasswordInput))],
      ~resolve=(_, (), _input) =>
      Lwt.return(Ok(None))
    )
  );
