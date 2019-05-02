open Library;

module PGOCaml = Database.PGOCaml;

let getAll = () => {
  let query = [%sqlf {|
    SELECT * FROM PROJECTS;
  |}];

  Database.runQuery(query);
};

let getById = id => {
  let query = [%sqlf {|
    SELECT * FROM PROJECTS WHERE ID = $id
  |}];

  Database.runQuery(query(~id));
};

let getByOrganizationId = organizationId => {
  let query = [%sqlf
    {|
    SELECT * FROM PROJECTS WHERE organization_id = $organizationId
  |}
  ];

  Database.runQuery(query(~organizationId));
};

let insert = (~name, ~webhook, ~organizationId) => {
  let query = [%sqlf
    {|
    INSERT INTO PROJECTS (name, webhook, organization_id) VALUES ($name, $webhook, $organizationId) RETURNING *;
  |}
  ];

  Database.runQuery(query(~name, ~webhook, ~organizationId));
};

let update = (~name, ~webhook, ~id) => {
  let query = [%sqlf
    {|
    UPDATE PROJECTS SET name = $name, webhook = $webhook WHERE id = $id RETURNING *;
  |}
  ];

  Database.runQuery(query(~name, ~webhook, ~id));
};
