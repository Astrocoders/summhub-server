open Graphql_lwt;
open GraphqlHelpers;
open Library;

type t = {
  id: string,
  name: string,
  webhook: string,
};

let resolver =
  Schema.(
    obj("Project", ~doc="Project of an organization", ~fields=_ =>
      [
        field("id", ~typ=non_null(guid), ~args=Arg.[], ~resolve=(_info, p: t) =>
          p.id
        ),
        field(
          "name", ~typ=non_null(string), ~args=Arg.[], ~resolve=(_info, p: t) =>
          p.name
        ),
        field(
          "webhook",
          ~typ=non_null(string),
          ~args=Arg.[],
          ~resolve=(_info, p: t) =>
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

module Connection = Connection.Make(Config);

type project = t;

module ModelConfig = {
  type t = project;
  let table = "projects";
  let parseRow = row => {id: row[0], name: row[2], webhook: row[3]};
};

module Model = Model.Make(ModelConfig);
