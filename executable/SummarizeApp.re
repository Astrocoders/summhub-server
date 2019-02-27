open Graphql_lwt;
open AppSchema;
open GraphqlHelpers;

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

let mockedMessages: list(Message.t) = [
  {id: 1, message: "Message", email: "email@provider.com"},
];

let role =
  User.(
    Schema.(
      enum(
        "Role",
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
      obj("Summary", ~doc="User's summary of notifications", ~fields=_ =>
        [
          field(
            "unread",
            ~doc="Count of unread items",
            ~typ=non_null(int),
            ~args=Arg.[],
            ~resolve=(_info, p) =>
            p.unread
          ),
          field(
            "total",
            ~doc="Total of notifications",
            ~typ=non_null(int),
            ~args=Arg.[],
            ~resolve=(_info, p) =>
            p.total
          ),
          field(
            "projects",
            ~doc="Total of projects",
            ~typ=non_null(int),
            ~args=Arg.[],
            ~resolve=(_info, p) =>
            p.projects
          ),
          field(
            "organizations",
            ~doc="Total of organizations",
            ~typ=non_null(int),
            ~args=Arg.[],
            ~resolve=(_info, p) =>
            p.organizations
          ),
        ]
      )
    )
  );

let notification =
  User.Notification.(
    Schema.(
      obj("Notification", ~doc="User notification", ~fields=_ =>
        [
          field(
            "id",
            ~doc="Unique notification identifier",
            ~typ=non_null(int),
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
            "payload",
            ~typ=non_null(string),
            ~args=Arg.[],
            ~resolve=(_info, p) =>
            p.payload
          ),
          Message.Connection.connectionResolver(
            "messages", (_info, p, first, after, last, before) =>
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
      "Timespan",
      ~fields=[arg("start", ~typ=string), arg("end", ~typ=string)],
      ~coerce=(start, end_) =>
      {start, end_}
    )
  );

let user =
  User.(
    Schema.(
      obj("User", ~doc="A user in the system", ~fields=_ =>
        [
          field(
            "id",
            ~doc="Unique user identifier",
            ~typ=non_null(int),
            ~args=Arg.[],
            ~resolve=(_info, p) =>
            p.id
          ),
          field(
            "name", ~args=Arg.[], ~typ=non_null(string), ~resolve=(_info, p) =>
            p.name
          ),
          field(
            "role", ~args=Arg.[], ~typ=non_null(role), ~resolve=(_info, p) =>
            p.role
          ),
          field(
            "summary",
            ~args=Arg.[],
            ~typ=non_null(summary),
            ~resolve=(_info, _p) =>
            mockedSummary
          ),
          field(
            "notifications",
            ~args=Arg.[arg("filter", ~typ=notificationsArg)],
            ~typ=list(non_null(notification)),
            ~resolve=(_info, _p, _filter) =>
            None
          ),
          Organization.Connection.connectionResolver(
            "organizations", (_info, _p, _first, _after, _last, _before) =>
            None
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
        RequestSignInLinkMutation.requestSignInLink,
        SendMessageMutation.sendMessage,
        CreateOrganizationMutation.createOrganization,
        RemoveOrganizationMutation.removeOrganization,
        UpdateOrganizationNameMutation.updateOrganizationName,
        AddOrganizationMemberMutation.addOrganizationMember,
        RemoveOrganizationMemberMutation.removeOrganizationMember,
        AddOrganizationProjectMutation.addOrganizationProject,
        UpdateOrganizationProjectNameMutation.updateOrganizationProjectName,
        RemoveOrganizationProjectMutation.removeOrganizationProject,
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
