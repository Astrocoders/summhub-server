open Graphql_lwt;
open GraphqlHelpers;
open Library;

type t = {
  id: string,
  name: string,
  webhook: string,
  organizationId: string,
  createdAt: string,
};

let typ =
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
  let nodeResolver = typ;
  let nodeName = "Project";
};

module Connection = Connection.Make(Config);

module Model = Models.Project;

let parseMember = member => {
  let (id, organizationId, name, webhook, createdAt) = member;
  {
    id,
    webhook,
    name,
    organizationId,
    createdAt: Util.Calendar.(defaultToNow(createdAt) |> toDateString),
  };
};

let processSingleResult = result =>
  List.length(result) > 0
    ? Some(parseMember(Array.of_list(result)[0])) : None;

let insert = (~name, ~webhook, ~organizationId) => {
  let%lwt result = Model.insert(~name, ~webhook, ~organizationId);
  processSingleResult(result) |> Lwt.return;
};
