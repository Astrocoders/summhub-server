open Graphql_lwt;
open GraphqlHelpers;
open Library;

type t = {
  id: string,
  email: string,
  message: string,
};

let typ =
  Schema.(
    obj("Message", ~doc="User notification message", ~fields=_ =>
      [
        field(
          "id",
          ~doc="Unique identifier of message",
          ~typ=non_null(guid),
          ~args=Arg.[],
          ~resolve=(_info, p) =>
          p.id
        ),
        field(
          "email",
          ~doc="Unique identifier of message",
          ~typ=non_null(string),
          ~args=Arg.[],
          ~resolve=(_info, p) =>
          p.email
        ),
        field(
          "message",
          ~doc="Unique identifier of message",
          ~typ=non_null(string),
          ~args=Arg.[],
          ~resolve=(_info, p) =>
          p.message
        ),
      ]
    )
  );

module Config = {
  type nodeType = t;
  type context = Context.t;
  let nodeResolver = typ;
  let nodeName = "Message";
};

module Connection = Connection.Make(Config);
