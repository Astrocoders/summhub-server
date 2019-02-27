open Graphql_lwt;

type resetPasswordInput = {
  token: string,
  password: string,
};

let resetPasswordInput =
  Schema.Arg.(
    obj(
      "ResetPasswordInput",
      ~fields=[
        arg("token", ~typ=non_null(string)),
        arg("password", ~typ=non_null(string)),
      ],
      ~coerce=(token, password) =>
      {token, password}
    )
  );

let resetPasswordPayload =
  Schema.(
    obj("ResetPasswordPayload", ~fields=( _: Graphql_lwt.Schema.typ(Context.t, option((option(string), option(string)))) ) =>
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

let resetPassword =
  Schema.(
    io_field(
      "resetPassword",
      ~typ=non_null(resetPasswordPayload),
      ~args=
        Arg.[
          arg("input", ~typ=non_null(resetPasswordInput)),
        ],
      ~resolve=(_, (), input) =>
      Lwt.return(Ok((None, None)))
    )
  );
