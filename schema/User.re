open Graphql_lwt;
open Library;

module Model = Models.User;

type role =
  | User
  | Admin;

type t = {
  id: string,
  email: string,
  role,
  createdAt: string,
};

let parseRole = roleString =>
  switch (roleString) {
  | "ADMIN" => Admin
  | _ => User
  };

let parseUser = user => {
  let (id, email, role, createdAt) = user;
  {
    id,
    email,
    role: parseRole(role),
    createdAt: Util.Calendar.timestampToDateString(createdAt),
  };
};

let processSingleResult = result =>
  List.length(result) > 0
    ? Some(parseUser(Array.of_list(result)[0])) : None;

let getByEmail = email => {
  let%lwt result = Model.getByEmail(email);
  processSingleResult(result) |> Lwt.return;
};

let insert = (~email, ~role=?, ()) => {
  let%lwt result = Model.insert(~email, ~role?, ());
  processSingleResult(result) |> Lwt.return;
};

let getById = id => {
  let%lwt result = Model.getById(id);
  processSingleResult(result) |> Lwt.return;
};

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
