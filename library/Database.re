module PGOCaml = PGOCaml_generic.Make(AsyncThread);

let connection = PGOCaml.connect;

let pool: Lwt_pool.t(PGOCaml.t(Hashtbl.t(string, bool))) =
  Lwt_pool.create(16, ~validate=PGOCaml.alive, connection);

let withPool = f => Lwt_pool.use(pool, f);

let runQuery = f => withPool(f);
