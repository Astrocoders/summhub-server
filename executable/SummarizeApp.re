open Graphql_lwt;
open AppSchema;

let mockedSummary: User.Summary.t = {
  unread: 1,
  total: 2,
  projects: 0,
  organizations: 0,
};

let mockedUser: User.t = {id: 1, name: "Alice", role: Admin};

let mockedNotifications: list(User.Notification.t) = [
  {
    id: 1,
    title: "Title",
    body: "Body",
    createdAt: "CreatedAt",
    icon: None,
    link: None,
    payload: "Payload",
  },
];

let mockedNotification: User.Notification.t = {
  id: 1,
  title: "Title",
  body: "Body",
  createdAt: "CreatedAt",
  icon: None,
  link: None,
  payload: "Payload",
};

let mockedMessages: list(User.Message.t) = [
  {id: 1, message: "Message", email: "email@provider.com"},
];

let role =
  User.(
    Schema.(
      enum(
        "role",
        ~doc="The role of a user",
        ~values=[
          enum_value("USER", ~value=User),
          enum_value("ADMIN", ~value=Admin),
        ],
      )
    )
  );

let summary =
  User.Summary.(
    Schema.(
      obj("summary", ~doc="User's summary of notifications", ~fields=_ =>
        [
          field(
            "unread",
            ~doc="Count of unread items",
            ~typ=non_null(int),
            ~args=Arg.[],
            ~resolve=(info, p) =>
            p.unread
          ),
          field(
            "total",
            ~doc="Total of notifications",
            ~typ=non_null(int),
            ~args=Arg.[],
            ~resolve=(info, p) =>
            p.total
          ),
          field(
            "projects",
            ~doc="Total of projects",
            ~typ=non_null(int),
            ~args=Arg.[],
            ~resolve=(info, p) =>
            p.projects
          ),
          field(
            "organizations",
            ~doc="Total of organizations",
            ~typ=non_null(int),
            ~args=Arg.[],
            ~resolve=(info, p) =>
            p.organizations
          ),
        ]
      )
    )
  );

let message =
  User.Message.(
    Schema.(
      obj("message", ~doc="User notification message", ~fields=_ =>
        [
          field(
            "id",
            ~doc="Unique identifier of message",
            ~typ=non_null(int),
            ~args=Arg.[],
            ~resolve=(info, p) =>
            p.id
          ),
          field(
            "email",
            ~doc="Unique identifier of message",
            ~typ=non_null(string),
            ~args=Arg.[],
            ~resolve=(info, p) =>
            p.email
          ),
          field(
            "message",
            ~doc="Unique identifier of message",
            ~typ=non_null(string),
            ~args=Arg.[],
            ~resolve=(info, p) =>
            p.message
          ),
        ]
      )
    )
  );

let messagesConnectionEdge =
  Connection.(
    Schema.(
      obj("messagesConnectionEdge", ~doc="Message Connection Edge", ~fields=_ =>
        [
          field(
            "cursor",
            ~doc="Cursor of connection edge",
            ~typ=non_null(string),
            ~args=Arg.[],
            ~resolve=(info, p: edge(User.Message.t)) =>
            p.cursor
          ),
          field(
            "node",
            ~doc="Node of connection edge",
            ~typ=non_null(message),
            ~args=Arg.[],
            ~resolve=(info, p: edge(User.Message.t)) =>
            p.node
          ),
        ]
      )
    )
  );

let messagesConnection =
  Connection.(
    Schema.(
      obj("messagesConnection", ~doc="Messages Connection", ~fields=_ =>
        [
          field(
            "edges",
            ~doc="Edges of connection",
            ~typ=list(non_null(messagesConnectionEdge)),
            ~args=Arg.[],
            ~resolve=(info, p: Connection.t(User.Message.t)) =>
            p.edges
          ),
          field(
            "pageInfo",
            ~doc="PageInfo of connection",
            ~typ=non_null(PageInfo.typeResolver),
            ~args=Arg.[],
            ~resolve=(info, p: Connection.t(User.Message.t)) =>
            p.pageInfo
          ),
        ]
      )
    )
  );

let notification =
  User.Notification.(
    Schema.(
      obj("notification", ~doc="User notification", ~fields=_ =>
        [
          field(
            "id",
            ~doc="Unique notification identifier",
            ~typ=non_null(int),
            ~args=Arg.[],
            ~resolve=(info, p) =>
            p.id
          ),
          field(
            "title",
            ~doc="Title notification",
            ~typ=non_null(string),
            ~args=Arg.[],
            ~resolve=(info, p) =>
            p.title
          ),
          field(
            "body",
            ~doc="Body notification",
            ~typ=non_null(string),
            ~args=Arg.[],
            ~resolve=(info, p) =>
            p.body
          ),
          field(
            "createdAt",
            ~typ=non_null(string),
            ~args=Arg.[],
            ~resolve=(info, p) =>
            p.createdAt
          ),
          field("icon", ~typ=string, ~args=Arg.[], ~resolve=(info, p) =>
            p.icon
          ),
          field("link", ~typ=string, ~args=Arg.[], ~resolve=(info, p) =>
            p.link
          ),
          field(
            "payload",
            ~typ=non_null(string),
            ~args=Arg.[],
            ~resolve=(info, p) =>
            p.payload
          ),
          field(
            "messages",
            ~typ=messagesConnection,
            ~args=
              Arg.[
                arg("first", ~typ=float),
                arg("after", ~typ=string),
                arg("last", ~typ=int),
                arg("before", ~typ=string),
              ],
            ~resolve=(_info, p, first, after, last, before) =>
            None
          ),
        ]
      )
    )
  );

type timespan = {
  start: option(string),
  end_: option(string),
};

let notificationsArg =
  Schema.Arg.(
    obj(
      "timespan",
      ~fields=[arg("start", ~typ=string), arg("end", ~typ=string)],
      ~coerce=(start, end_) =>
      {start, end_}
    )
  );

let user =
  User.(
    Schema.(
      obj("user", ~doc="A user in the system", ~fields=_ =>
        [
          field(
            "id",
            ~doc="Unique user identifier",
            ~typ=non_null(int),
            ~args=Arg.[],
            ~resolve=(info, p) =>
            p.id
          ),
          field(
            "name", ~args=Arg.[], ~typ=non_null(string), ~resolve=(info, p) =>
            p.name
          ),
          field(
            "role", ~args=Arg.[], ~typ=non_null(role), ~resolve=(info, p) =>
            p.role
          ),
          field(
            "summary",
            ~args=Arg.[],
            ~typ=non_null(summary),
            ~resolve=(info, p) =>
            mockedSummary
          ),
          field(
            "notifications",
            ~args=Arg.[arg("filter", ~typ=notificationsArg)],
            ~typ=list(non_null(notification)),
            ~resolve=(_info, p, filter) =>
            None
          ),
          field(
            "organizations",
            ~args=Arg.[],
            ~typ=list(non_null(Organization.typeResolver)),
            ~resolve=(info: Context.t, p) => None
          ),
        ]
      )
    )
  );

let schema =
  Graphql_lwt.Schema.(
    schema(
      [
        io_field(
          "currentUser",
          ~typ=user,
          ~args=Arg.[],
          ~resolve=(info: Context.t, ()) =>
          Lwt.return(Ok(info.user))
        ),
      ],
      ~mutations=[
        SignInMutation.signIn,
        ForgotPasswordMutation.forgotPassword,
        ResetPasswordMutation.resetPassword,
      ],
    )
  );

module Graphql_cohttp_lwt =
  Graphql_cohttp.Make(Graphql_lwt.Schema, Cohttp_lwt.Body);

let _ =
  Lwt_main.run(
    {
      let callback =
        Graphql_cohttp_lwt.make_callback(
          _req => Context.{user: Some(mockedUser)},
          schema,
        );
      let server = Cohttp_lwt_unix.Server.make(~callback, ());
      let port = 3000;
      let mode = `TCP(`Port(port));
      Console.log(
        <Pastel>
          <Pastel> "Running at " </Pastel>
          <Pastel color=Cyan> {string_of_int(port)} </Pastel>
        </Pastel>,
      );
      Cohttp_lwt_unix.Server.create(~mode, server);
    },
  );
