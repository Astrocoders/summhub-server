open Library;

module PGOCaml = Database.PGOCaml;

let getAll = () => {
  let query = [%sqlf {|
    SELECT * FROM ORGANIZATIONS;
  |}];

  Database.runQuery(query);
};

let getById = id => {
  let query = [%sqlf {|
    SELECT * FROM ORGANIZATIONS WHERE ID = $id
  |}];

  Database.runQuery(query(~id));
};

let getByUserId = userId => {
  let query = [%sqlf
    {|
    SELECT * FROM ORGANIZATIONS WHERE user_id = $userId
  |}
  ];

  Database.runQuery(query(~userId));
};

let insert = (~name, ~userId, ()) => {
  let query = [%sqlf
    {|
    INSERT INTO ORGANIZATIONS (name, user_id) VALUES ($name, $userId) RETURNING *;
  |}
  ];

  Database.runQuery(query(~name, ~userId));
};

let update = (~id, ~name) => {
  let query = [%sqlf
    {|
    UPDATE ORGANIZATIONS SET name = $name WHERE id = $id RETURNING *;
  |}
  ];

  Database.runQuery(query(~name, ~id));
};
