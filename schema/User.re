open Graphql_lwt;
open Library;

module Summary = {
  type t = {
    unread: int,
    total: int,
    projects: int,
    organizations: int,
  };
};

module Notification = {
  type t = {
    id: string,
    title: string,
    body: string,
    createdAt: string,
    icon: option(string),
    link: option(string),
    payload: string,
  };
};

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
