open Graphql_lwt;

module PageInfo = {
  type t = {
    startCursor: option(string),
    hasPreviousPage: bool,
    hasNextPage: bool,
    endCursor: option(string),
    total: option(int),
  };

  let typeResolver =
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
          field("total", ~doc="", ~typ=int, ~args=Arg.[], ~resolve=(info: Context.t, p) =>
            p.total
          ),
        ]
      )
    );
};

type edge('a) = {
  cursor: string,
  node: 'a,
};

type t('a) = {
  pageInfo: PageInfo.t,
  edges: option(list(edge('a))),
};
