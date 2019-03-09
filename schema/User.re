open Graphql_lwt;
open Library;

type role =
  | User
  | Admin;

type t = {
  id: string,
  email: string,
  role,
};

type user = t;

module ModelConfig = {
  type t = user;
  let table = "app_users";
  let parseRow = row => {
    id: row[0],
    email: row[1],
    role: row[2] == "ADMIN" ? Admin : User,
  };
};

module Model = Model.Make(ModelConfig);

let getByEmail = (connection, email) =>
  Model.findOne(~connection, ~clause="email=" ++ "'" ++ email ++ "'", ())
  |> Lwt_main.run;

let createUser = (connection, email) =>
  Model.create(
    ~connection,
    ~fields=["email"],
    ~values=[Database.wrapColumnValue(email)],
    (),
  )
  |> Lwt_main.run;
