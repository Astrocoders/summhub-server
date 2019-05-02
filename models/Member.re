open Library;

module PGOCaml = Database.PGOCaml;

let getAll = () => {
  let query = [%sqlf {|
    SELECT * FROM MEMBERS;
  |}];

  Database.runQuery(query);
};

let getById = id => {
  let query = [%sqlf {|
    SELECT * FROM MEMBERS WHERE ID = $id
  |}];

  Database.runQuery(query(~id));
};

let getByOrganizationId = organizationId => {
  let query = [%sqlf
    {|
    SELECT * FROM MEMBERS WHERE organization_id = $organizationId
  |}
  ];

  Database.runQuery(query(~organizationId));
};

let insert = (~email, ~organizationId, ()) => {
  let query = [%sqlf
    {|
    INSERT INTO MEMBERS (email, organization_id) VALUES ($email, $organizationId) RETURNING *;
  |}
  ];

  Database.runQuery(query(~email, ~organizationId));
};

let update = (~id, ~email) => {
  let query = [%sqlf
    {|
    UPDATE MEMBERS SET email = $email WHERE id = $id RETURNING *;
  |}
  ];

  Database.runQuery(query(~email, ~id));
};
