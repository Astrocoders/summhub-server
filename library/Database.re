open Ezpostgresql;

exception Database_not_configured;

let wrapColumnValue = value => "'" ++ value ++ "'";

let conninfo =
  try (Sys.getenv("DATABASE_URL")) {
  | Not_found => raise(Database_not_configured)
  };
let pool = Pool.create(~conninfo, ~size=10, ());
