open Graphql_lwt;
open GraphqlHelpers;

type sendMessageInput = {
  notificationId: string,
  message: string,
};

let sendMessageInput =
  Schema.Arg.(
    obj(
      "SendMessageInput",
      ~fields=[
        arg("notificationId", ~typ=non_null(guid)),
        arg("message", ~typ=non_null(string)),
      ],
      ~coerce=(notificationId, message) =>
      {notificationId, message}
    )
  );

let payload =
  Schema.(
    obj(
      "SendMessagePayload",
      ~fields=(
                _:
                  Graphql_lwt.Schema.typ(
                    Context.t,
                    option((option(string), option(Message.t))),
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
          "message",
          ~typ=Message.typ,
          ~args=Arg.[],
          ~resolve=(_: Context.t, (_, message)) =>
          message
        ),
      ]
    )
  );

let sendMessage =
  Schema.(
    io_field(
      "sendMessage",
      ~typ=non_null(payload),
      ~args=Arg.[arg("input", ~typ=non_null(sendMessageInput))],
      ~resolve=(info, (), _input) =>
      switch (info.user) {
      | Some(user) => Lwt.return(Ok((None, None)))
      | None => Lwt.return(Ok((Some(Errors.unauthorized), None)))
      }
    )
  );
