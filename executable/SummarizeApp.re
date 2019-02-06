open Graphql_lwt;

type role =
  | User
  | Admin;
type user = {
  id: int,
  name: string,
  role,
};

let users = [
  {id: 1, name: "Alice", role: Admin},
  {id: 2, name: "Bob", role: User},
];

let role =
  Schema.(
    enum(
      "role",
      ~doc="The role of a user",
      ~values=[
        enum_value("USER", ~value=User),
        enum_value("ADMIN", ~value=Admin),
      ],
    )
  );

let user =
  Schema.(
    obj("user", ~doc="A user in the system", ~fields=_ =>
      [
        field(
          "id",
          ~doc="Unique user identifier",
          ~typ=non_null(int),
          ~args=Arg.[],
          ~resolve=(info, p) =>
          p.id
        ),
        field(
          "name", 
          ~args=Arg.[],
          ~typ=non_null(string),
          ~resolve=(info, p) => p.name
        ),
        field(
          "role", 
          ~args=Arg.[],
          ~typ=non_null(role),
          ~resolve=(info, p) => p.role
        ),
      ]
    )
  );

let schema =
  Graphql_lwt.Schema.(
    schema([
      io_field(
        "users", 
        ~typ=non_null(list(non_null(user))), 
        ~args=Arg.[],
        ~resolve=( (info, ()) => Lwt.return( ( Ok( users ) ) ) )
      ),
    ])
  );


module Graphql_cohttp_lwt =
  Graphql_cohttp.Make(Graphql_lwt.Schema, Cohttp_lwt.Body);

let _ = Lwt_main.run({
  let callback = Graphql_cohttp_lwt.make_callback(_req => (), schema);
  let server = Cohttp_lwt_unix.Server.make(~callback, ());
  let port = 3000
  let mode = `TCP(`Port(port));
  Console.log(
    <Pastel>
      <Pastel>"Running at "</Pastel>
      <Pastel color=Cyan> { string_of_int(port) } </Pastel>
    </Pastel>
  );
  Cohttp_lwt_unix.Server.create(~mode, server);
});
