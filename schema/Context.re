type summary = {
  unread: int,
  total: int,
  projects: int,
  organizations: int,
};

type role =
  | User
  | Admin;

type user = {
  id: int,
  name: string,
  role,
  summary,
};

type t = {user: option(user)};
