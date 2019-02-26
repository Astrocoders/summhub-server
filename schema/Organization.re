open Graphql_lwt;

module Project = {
  type t = {
    id: int,
    name: string,
    webhook: string,
  };

  type project = t;

  let typeResolver =
    Schema.(
      obj("project", ~doc="Project of an organization", ~fields=_ =>
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
            "webhook",
            ~typ=non_null(string),
            ~args=Arg.[],
            ~resolve=(info: Context.t, p: t) =>
            p.webhook
          ),
        ]
      )
    );

  let connectionEdge =
    Connection.(
      Schema.(
        obj("projectConnectionEdge", ~doc="Project Connection Edge", ~fields=_ =>
          [
            field(
              "cursor",
              ~doc="Cursor of connection edge",
              ~typ=non_null(string),
              ~args=Arg.[],
              ~resolve=(info, p: edge(project)) =>
              p.cursor
            ),
            field(
              "node",
              ~doc="Node of connection edge",
              ~typ=non_null(typeResolver),
              ~args=Arg.[],
              ~resolve=(info, p: edge(project)) =>
              p.node
            ),
          ]
        )
      )
    );

  let connection =
    Schema.(
      obj("projectConnection", ~doc="Projects Connection", ~fields=_ =>
        [
          field(
            "edges",
            ~doc="Edges of connection",
            ~typ=list(non_null(connectionEdge)),
            ~args=Arg.[],
            ~resolve=(info, p: Connection.t(project)) =>
            p.edges
          ),
          field(
            "pageInfo",
            ~doc="PageInfo of connection",
            ~typ=non_null(Connection.PageInfo.typeResolver),
            ~args=Arg.[],
            ~resolve=(info, p: Connection.t(project)) =>
            p.pageInfo
          ),
        ]
      )
    );
};

module Member = {
  type t = {
    id: int,
    email: string,
  };

  type member = t;

  let typeResolver =
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

  let connectionEdge =
    Connection.(
      Schema.(
        obj("memberConnectionEdge", ~doc="Member Connection Edge", ~fields=_ =>
          [
            field(
              "cursor",
              ~doc="Cursor of connection edge",
              ~typ=non_null(string),
              ~args=Arg.[],
              ~resolve=(info, p: edge(member)) =>
              p.cursor
            ),
            field(
              "node",
              ~doc="Node of connection edge",
              ~typ=non_null(typeResolver),
              ~args=Arg.[],
              ~resolve=(info, p: edge(member)) =>
              p.node
            ),
          ]
        )
      )
    );

  let connection =
    Schema.(
      obj("messagesConnection", ~doc="Messages Connection", ~fields=_ =>
        [
          field(
            "edges",
            ~doc="Edges of connection",
            ~typ=list(non_null(connectionEdge)),
            ~args=Arg.[],
            ~resolve=(info, p: Connection.t(member)) =>
            p.edges
          ),
          field(
            "pageInfo",
            ~doc="PageInfo of connection",
            ~typ=non_null(Connection.PageInfo.typeResolver),
            ~args=Arg.[],
            ~resolve=(info, p: Connection.t(member)) =>
            p.pageInfo
          ),
        ]
      )
    );
};

type t = {
  id: int,
  name: string,
  createdAt: string,
};

let typeResolver =
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
          ~typ=Member.connection,
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
          ~typ=Project.connection,
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
