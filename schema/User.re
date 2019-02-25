type role =
  | User
  | Admin;

type summary = {
  unread: int,
  total: int,
  projects: int,
  organizations: int,
};

type t = {
  id: int,
  name: string,
  role,
};
