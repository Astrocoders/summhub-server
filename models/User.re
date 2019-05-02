open Library;

module PGOCaml = Database.PGOCaml;

let getAll = () => {
  let query = [%sqlf {|
    SELECT * FROM APP_USERS;
  |}];

  Database.runQuery(query);
};

let getById = id => {
  let query = [%sqlf {|
    SELECT * FROM APP_USERS WHERE ID = $id
  |}];

  Database.runQuery(query(~id));
};

let getByEmail = email => {
  let query = [%sqlf {|
    SELECT * FROM APP_USERS WHERE EMAIL = $email
  |}];

  Database.runQuery(query(~email));
};

let insert = (~email, ~role="USER", ()) => {
  let query = [%sqlf
    {|
    INSERT INTO APP_USERS (email, role) VALUES ($email, $role) RETURNING *;
  |}
  ];

  Database.runQuery(query(~email, ~role));
};

let update = (~id, ~email, ~role) => {
  let query = [%sqlf
    {|
    UPDATE APP_USERS SET email = $email, role = $role WHERE id = $id RETURNING *;
  |}
  ];

  Database.runQuery(query(~id, ~email, ~role));
};
