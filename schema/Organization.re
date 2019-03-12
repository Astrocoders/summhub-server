open Graphql_lwt;
open GraphqlHelpers;
open Library;

type t = {
  id: string,
  name: string,
  createdAt: string,
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

type organization = t;

module ModelConfig = {
  type t = organization;
  let table = "organizations";
  let parseRow = row => {id: row[0], name: row[1], createdAt: row[2]};
};

module Model = Model.Make(ModelConfig);

/* TODO: Fetch random name  */
let insert = (connection, ~userId) =>
  Model.insert(
    ~connection,
    ~fields=[
      ("name", Database.wrapStringValue("Organization Created")),
      ("user_id", Database.wrapStringValue(userId)),
    ],
    (),
  );
