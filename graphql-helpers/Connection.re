open Graphql_lwt;

module type Config = {
  type nodeType;
  type context;
  let nodeResolver: Schema.typ(context, option(nodeType));
  let nodeName: string;
};

module Create = (Config: Config) => {
  module PageInfo = {
    type t = {
      startCursor: option(string),
      hasPreviousPage: bool,
      hasNextPage: bool,
      endCursor: option(string),
      total: option(int),
    };

    let resolver =
      Schema.(
        obj("pageInfo", ~doc="Connections page info", ~fields=_ =>
          [
            field(
              "startCursor",
              ~doc="",
              ~typ=string,
              ~args=Arg.[],
              ~resolve=(info, p) =>
              p.startCursor
            ),
            field(
              "hasPreviousPage",
              ~doc="",
              ~typ=non_null(bool),
              ~args=Arg.[],
              ~resolve=(info, p) =>
              p.hasPreviousPage
            ),
            field(
              "hasNextPage",
              ~doc="",
              ~typ=non_null(bool),
              ~args=Arg.[],
              ~resolve=(info, p) =>
              p.hasNextPage
            ),
            field(
              "endCursor",
              ~doc="",
              ~typ=string,
              ~args=Arg.[],
              ~resolve=(info, p) =>
              p.endCursor
            ),
            field(
              "total",
              ~doc="",
              ~typ=int,
              ~args=Arg.[],
              ~resolve=(info: Config.context, p) =>
              p.total
            ),
          ]
        )
      );
  };

  type edge = {
    cursor: string,
    node: Config.nodeType,
  };

  type connection = {
    pageInfo: PageInfo.t,
    edges: option(list(edge)),
  };

  let edgeResolver =
    Schema.(
      obj(Config.nodeName ++ "ConnectionEdge", ~fields=_ =>
        [
          field(
            "cursor",
            ~doc="Cursor of connection edge",
            ~typ=non_null(string),
            ~args=Arg.[],
            ~resolve=(info: Config.context, p: edge) =>
            p.cursor
          ),
          field(
            "node",
            ~doc="Node of connection edge",
            ~typ=non_null(Config.nodeResolver),
            ~args=Arg.[],
            ~resolve=(info: Config.context, p: edge) =>
            p.node
          ),
        ]
      )
    );
  let connectionResolver =
    Schema.(
      obj(Config.nodeName ++ "Connection", ~fields=_ =>
        [
          field(
            "edges",
            ~doc="Edges of connection",
            ~typ=list(non_null(edgeResolver)),
            ~args=Arg.[],
            ~resolve=(info: Config.context, p: connection) =>
            p.edges
          ),
          field(
            "pageInfo",
            ~doc="PageInfo of connection",
            ~typ=non_null(PageInfo.resolver),
            ~args=Arg.[],
            ~resolve=(info: Config.context, p: connection) =>
            p.pageInfo
          ),
        ]
      )
    );
};
