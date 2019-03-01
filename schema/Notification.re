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

let resolver =
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

type notification = t;

module ModelConfig = {
  type t = notification;
  let table = "notifications";
  let parseRow = row => {
    id: row[0],
    title: row[2],
    body: row[3],
    icon: String.length(row[4]) > 0 ? Some(row[4]) : None,
    link: String.length(row[5]) > 0 ? Some(row[5]) : None,
    payload: row[6],
    createdAt: row[7],
  };
};

module Model = Model.Make(ModelConfig);
