open Graphql_lwt;
open Library;

type role =
  | User
  | Admin;

type t = {
  id: string,
  email: string,
  role,
};

type user = t;

module ModelConfig = {
  type t = user;
  let table = "app_users";
  let parseRow = row => {
    id: row[0],
    email: row[1],
    role: row[2] == "ADMIN" ? Admin : User,
  };
};

module Model = Model.Make(ModelConfig);

let getByEmail = (connection, email) =>
  Model.findOne(~connection, ~clause="email=" ++ "'" ++ email ++ "'", ());

let insert = (connection, ~email, ~role) =>
  Model.insert(
    ~connection,
    ~fields=[
      ("email", Database.wrapStringValue(email)),
      ("role", Database.wrapStringValue(role)),
    ],
    (),
  );

let sendSignInLink = user =>
  Sendgrid.sendEmail(
    ~to_=user.email,
    ~subject="Summhub - Login Access Link",
    ~content=
      "<a href=\""
      ++ Constants.universalLinkAddress
      ++ "/sign-in/\""
      ++ Auth.encodeToken(user.id)
      ++ " > Click here to sign in </a>",
  );
