type t = {
  user: option(User.t),
  connection: Lwt_pool.t(Ezpostgresql.connection),
};
