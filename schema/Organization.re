open Graphql_lwt;
open GraphqlHelpers;
open Library;

module Model = Models.Organization;

type t = {
  id: string,
  name: string,
  createdAt: string,
  userId: string,
};

let typ =
  Schema.(
    obj("Organization", ~doc="User organization", ~fields=_ =>
      [
        field(
          "id",
          ~typ=non_null(guid),
          ~args=Arg.[],
          ~resolve=(_info: Context.t, p: t) =>
          p.id
        ),
        field(
          "name",
          ~typ=non_null(string),
          ~args=Arg.[],
          ~resolve=(_info: Context.t, p: t) =>
          p.name
        ),
        field(
          "createdAt",
          ~typ=non_null(string),
          ~args=Arg.[],
          ~resolve=(_info: Context.t, p: t) =>
          p.createdAt
        ),
        Member.Connection.connectionResolver(
          "members", (_info, _p, _first, _after, _last, _before) =>
          None
        ),
        Project.Connection.connectionResolver(
          "projects", (_info, _p, _first, _after, _last, _before) =>
          None
        ),
      ]
    )
  );

module Config = {
  type nodeType = t;
  type context = Context.t;
  let nodeResolver = typ;
  let nodeName = "Organization";
};

module Connection = Connection.Make(Config);

let parseOrganization = organization => {
  let (id, name, createdAt, userId) = organization;
  {
    id,
    name,
    createdAt: Util.Calendar.timestampToDateString(createdAt),
    userId,
  };
};

let processSingleResult = result =>
  List.length(result) > 0
    ? Some(parseOrganization(Array.of_list(result)[0])) : None;

/* TODO: Fetch random name  */
let insert = (~name="Organization Created", ~userId, ()) => {
  let%lwt result = Model.insert(~name, ~userId, ());
  processSingleResult(result) |> Lwt.return;
};
