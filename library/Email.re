open Cohttp_lwt_unix;

exception Sendgrid_api_key_not_provided;

let apiKey =
  try (Sys.getenv("SENDGRID_API_KEY")) {
  | Not_found => raise(Sendgrid_api_key_not_provided)
  };

let sendEmail = (~from, ~to_, ~content, ~subject) => {
  Client.post(
    ~headers=
      Cohttp.Header.(
        init()
        |> (header => add(header, "Authorization", "Bearer " ++ apiKey))
        |> (header => add(header, "Content-Type", "application/json"))
      ),
    ~body=
      Cohttp_lwt__.Body.of_string(
        Yojson.Raw.from_string(
          "{ from: { email: "
          ++ from
          ++ "}},"
          ++ "subject: "
          ++ subject
          ++ ","
          ++ "content: [ { type: text/plain, value: "
          ++ content
          ++ "}],"
          ++ "personalizations: [ { to: [ { email: "
          ++ to_
          ++ "}]}]}",
        )
        |> Yojson.Raw.to_string,
      ),
    Uri.of_string("https://api.sendgrid.com/v3/mail/send"),
  );
};
