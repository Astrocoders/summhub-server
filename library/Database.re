open Ezpostgresql;

let conninfo =
  try (Sys.getenv("DATABASE_URL")) {
  | Not_found => "postgresql://localhost:5432/summhub_dev"
  };
let pool = Pool.create(~conninfo, ~size=10, ());
