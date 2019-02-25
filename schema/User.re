module Summary = {
  type t = {
    unread: int,
    total: int,
    projects: int,
    organizations: int,
  };
};

module Message = {
  type t = {
    id: int,
    email: string,
    message: string,
  };
};

module Notification = {
  type t = {
    id: int,
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
  id: int,
  name: string,
  role,
};
