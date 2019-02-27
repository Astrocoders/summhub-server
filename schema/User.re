open Graphql_lwt;

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
