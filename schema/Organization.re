open Graphql_lwt;
open GraphqlHelpers;

module Project = {
  type t = {
    id: int,
    name: string,
    webhook: string,
  };

  let resolver =
    Schema.(
      obj("Project", ~doc="Project of an organization", ~fields=_ =>
        [
          field("id", ~typ=non_null(int), ~args=Arg.[], ~resolve=(info, p: t) =>
            p.id
          ),
          field(
            "name",
            ~typ=non_null(string),
            ~args=Arg.[],
            ~resolve=(info, p: t) =>
            p.name
          ),
          field(
            "webhook",
            ~typ=non_null(string),
            ~args=Arg.[],
            ~resolve=(info, p: t) =>
            p.webhook
          ),
        ]
      )
    );

  module Config = {
    type nodeType = t;
    type context = Context.t;
    let nodeResolver = resolver;
    let nodeName = "Project";
  };

  module Connection = Connection.Create(Config);
};

module Member = {
  type t = {
    id: int,
    email: string,
  };

  let resolver =
    Schema.(
      obj("Member", ~doc="Member of an organization", ~fields=_ =>
        [
          field("id", ~typ=non_null(int), ~args=Arg.[], ~resolve=(info, p) =>
            p.id
          ),
          field(
            "email", ~typ=non_null(string), ~args=Arg.[], ~resolve=(info, p) =>
            p.email
          ),
        ]
      )
    );

  module Config = {
    type nodeType = t;
    type context = Context.t;
    let nodeResolver = resolver;
    let nodeName = "Member";
  };

  module Connection = Connection.Create(Config);
};

type t = {
  id: int,
  name: string,
  createdAt: string,
};

let resolver =
  Schema.(
    obj("Organization", ~doc="User organization", ~fields=_ =>
      [
        field(
          "id",
          ~typ=non_null(int),
          ~args=Arg.[],
          ~resolve=(info: Context.t, p: t) =>
          p.id
        ),
        field(
          "name",
          ~typ=non_null(string),
          ~args=Arg.[],
          ~resolve=(info: Context.t, p: t) =>
          p.name
        ),
        field(
          "createdAt",
          ~typ=non_null(string),
          ~args=Arg.[],
          ~resolve=(info: Context.t, p: t) =>
          p.createdAt
        ),
        Member.Connection.connectionResolver(
          "members", (_info, p, first, after, last, before) =>
          None
        ),
        Project.Connection.connectionResolver(
          "projects", (_info, p, first, after, last, before) =>
          None
        ),
      ]
    )
  );

module Config = {
  type nodeType = t;
  type context = Context.t;
  let nodeResolver = resolver;
  let nodeName = "Organization";
};

module Connection = Connection.Create(Config);
