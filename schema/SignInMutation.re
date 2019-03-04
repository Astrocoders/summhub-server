open Graphql_lwt;

let payload =
  Schema.(
    obj(
      "SignInPayload",
      ~fields=(
                _:
                  Graphql_lwt.Schema.typ(
                    Context.t,
                    option((option(string), option(string))),
                  ),
              ) =>
      [
        field("error", ~typ=string, ~args=Arg.[], ~resolve=(_, (error, _)) =>
          error
        ),
        field("token", ~typ=string, ~args=Arg.[], ~resolve=(_, (_, token)) =>
          token
        ),
      ]
    )
  );

let input =
  Schema.Arg.(
    obj(
      "SignInInput",
      ~fields=[
        arg("email", ~typ=non_null(string)),
        arg("password", ~typ=non_null(string)),
      ],
      ~coerce=(email, password) =>
      (email, password)
    )
  );

let signIn =
  Schema.(
    io_field(
      "signIn",
      ~typ=non_null(payload),
      ~args=Arg.[arg("input", ~typ=non_null(input))],
      ~resolve=(_, (), (_email, _password)) => {
        Lwt.return(Ok((None, None)))
      }
    )
  );
