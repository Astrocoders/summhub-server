open Graphql_lwt;
open GraphqlHelpers;
open Library;

type t = {
  id: string,
  title: string,
  body: string,
  createdAt: string,
  icon: option(string),
  link: option(string),
  payload: string,
};

let typ =
  Schema.(
    obj("Notification", ~doc="User notification", ~fields=_ =>
      [
        field(
          "id",
          ~doc="Unique notification identifier",
          ~typ=non_null(guid),
          ~args=Arg.[],
          ~resolve=(_info, p) =>
          p.id
        ),
        field(
          "title",
          ~doc="Title notification",
          ~typ=non_null(string),
          ~args=Arg.[],
          ~resolve=(_info, p) =>
          p.title
        ),
        field(
          "body",
          ~doc="Body notification",
          ~typ=non_null(string),
          ~args=Arg.[],
          ~resolve=(_info, p) =>
          p.body
        ),
        field(
          "createdAt",
          ~typ=non_null(string),
          ~args=Arg.[],
          ~resolve=(_info, p) =>
          p.createdAt
        ),
        field("icon", ~typ=string, ~args=Arg.[], ~resolve=(_info, p) =>
          p.icon
        ),
        field("link", ~typ=string, ~args=Arg.[], ~resolve=(_info, p) =>
          p.link
        ),
        field(
          "payload", ~typ=non_null(string), ~args=Arg.[], ~resolve=(_info, p) =>
          p.payload
        ),
        Message.Connection.connectionResolver(
          "messages", (_info, p, first, after, last, before) =>
          None
        ),
      ]
    )
  );
