open Graphql_lwt;
open GraphqlHelpers;
open Library;

type t = {
  id: string,
  email: string,
  organizationId: string,
  createdAt: string,
};

let typ =
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
  let nodeResolver = typ;
  let nodeName = "Member";
};

module Connection = Connection.Make(Config);

module Model = Models.Member;

let parseMember = member => {
  let (id, organizationId, email, createdAt) = member;
  {
    id,
    email,
    organizationId,
    createdAt: Util.Calendar.timestampToDateString(createdAt),
  };
};

let processSingleResult = result =>
  List.length(result) > 0
    ? Some(parseMember(Array.of_list(result)[0])) : None;

let insert = (~email, ~organizationId, ()) => {
  let%lwt result = Model.insert(~email, ~organizationId, ());
  processSingleResult(result) |> Lwt.return;
};
