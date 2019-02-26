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
      obj("project", ~doc="Project of an organization", ~fields=_ =>
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
    let nodeName = "project";
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
      obj("member", ~doc="Member of an organization", ~fields=_ =>
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
    let nodeName = "member";
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
    obj("organization", ~doc="User organization", ~fields=_ =>
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
        field(
          "members",
          ~typ=Member.Connection.connectionResolver,
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
        field(
          "projects",
          ~typ=Project.Connection.connectionResolver,
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
  );

module Config = {
  type nodeType = t;
  type context = Context.t;
  let nodeResolver = resolver;
  let nodeName = "organization";
};

module Connection = Connection.Create(Config);
