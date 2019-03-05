open Graphql_lwt;
open Library;

type role =
  | User
  | Admin;

type t = {
  id: string,
  name: string,
  role,
};

type user = t;

module ModelConfig = {
  type t = user;
  let table = "app_users";
  let parseRow = row => {
    id: row[0],
    name: row[1],
    role: row[2] == "ADMIN" ? Admin : User,
  };
};

module Model = Model.Make(ModelConfig);
