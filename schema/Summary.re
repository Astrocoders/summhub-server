open Graphql_lwt;
open Library;

type t = {
  unread: int,
  total: int,
  projects: int,
  organizations: int,
};

let typ =
  Schema.(
    obj("Summary", ~doc="User's summary of notifications", ~fields=_ =>
      [
        field(
          "unread",
          ~doc="Count of unread items",
          ~typ=non_null(int),
          ~args=Arg.[],
          ~resolve=(_info: Context.t, p) =>
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
  );
