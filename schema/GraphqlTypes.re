open Graphql_lwt;

let errorPayload =
  Schema.(
    obj(
      "ErrorPayload",
      ~fields=(
                _: Graphql_lwt.Schema.typ(Context.t, option(option(string))),
              ) =>
      [
        field(
          "error", ~typ=string, ~args=Arg.[], ~resolve=(_: Context.t, error) =>
          error
        ),
      ]
    )
  );
