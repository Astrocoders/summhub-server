open Graphql_lwt;
open AppSchema;

type role =
  | User
  | Admin;

type summary = {
  unread: int,
  total: int,
  projects: int,
  organizations: int,
};

type user = {
  id: int,
  name: string,
  role,
  summary,
};

let mockedUser: user = {
  id: 1,
  name: "Alice",
  role: Admin,
  summary: {
    unread: 0,
    total: 0,
    projects: 0,
    organizations: 0,
  },
};

let role =
  Schema.(
    enum(
      "role",
      ~doc="The role of a user",
      ~values=[
        enum_value("USER", ~value=User),
        enum_value("ADMIN", ~value=Admin),
      ],
    )
  );

let summary =
  Schema.(
    obj("summary", ~doc="User's summary of notifications", ~fields=_ =>
      [
        field(
          "unread",
          ~doc="Count of unread items",
          ~typ=non_null(int),
          ~args=Arg.[],
          ~resolve=(info, p: summary) =>
          p.unread
        ),
        field(
          "total",
          ~doc="Total of notifications",
          ~typ=non_null(int),
          ~args=Arg.[],
          ~resolve=(info, p: summary) =>
          p.total
        ),
        field(
          "projects",
          ~doc="Total of projects",
          ~typ=non_null(int),
          ~args=Arg.[],
          ~resolve=(info, p: summary) =>
          p.projects
        ),
        field(
          "organizations",
          ~doc="Total of organizations",
          ~typ=non_null(int),
          ~args=Arg.[],
          ~resolve=(info, p: summary) =>
          p.organizations
        ),
      ]
    )
  );

let user =
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
        field("role", ~args=Arg.[], ~typ=non_null(role), ~resolve=(info, p) =>
          p.role
        ),
        field(
          "summary", ~args=Arg.[], ~typ=non_null(summary), ~resolve=(info, p) =>
          p.summary
        ),
      ]
    )
  );

let schema =
  Graphql_lwt.Schema.(
    schema(
      [
        io_field("currentUser", ~typ=user, ~args=Arg.[], ~resolve=(_info, ()) =>
          Lwt.return(Ok(Some(mockedUser)))
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
          _req => Context.{user: None},
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
