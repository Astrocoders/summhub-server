open Graphql_lwt;
open GraphqlHelpers;
open Library;

type t = {
  id: string,
  email: string,
};

let resolver =
  Schema.(
    obj("Member", ~doc="Member of an organization", ~fields=_ =>
      [
        field("id", ~typ=non_null(guid), ~args=Arg.[], ~resolve=(_info, p) =>
          p.id
        ),
        field(
          "email", ~typ=non_null(string), ~args=Arg.[], ~resolve=(_info, p) =>
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

module Connection = Connection.Make(Config);

type member = t;

module ModelConfig = {
  type t = member;
  let table = "members";
  let parseRow = row => {
    id: row[0],
    email: row[2]
  };
};

module Model = Model.Make(ModelConfig);
