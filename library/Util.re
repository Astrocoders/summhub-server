exception Token_secret_not_provided;

let foo = () => print_endline("Hello");

let getTokenSecret = () =>
  try (Sys.getenv("JWT_SECRET")) {
  | Not_found => raise(Token_secret_not_provided)
  };

let decodeToken = token => {
  open Jwt;
  let payload = t_of_token(token) |> payload_of_t;
  find_claim(sub, payload);
};

let encodeToken = userId => {
  open Jwt;
  let header = header_of_algorithm_and_typ(HS256(getTokenSecret()), "JWT");
  let payload =
    empty_payload
    |> add_claim(iss, "summhub_server")
    |> add_claim(sub, userId);
  t_of_header_and_payload(header, payload) |> token_of_t;
};
