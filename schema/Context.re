
type role =
  | User
  | Admin;

type user = {
  id: int,
  name: string,
  role,
};

type t = {user: option(user)};
