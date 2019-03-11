open Cohttp_lwt_unix;

exception Sendgrid_api_key_not_provided;
exception Sendgrid_sender_email_not_provided;

let apiKey =
  try (Sys.getenv("SENDGRID_API_KEY")) {
  | Not_found => raise(Sendgrid_api_key_not_provided)
  };

let senderEmail =
  try (Sys.getenv("SENDGRID_SENDER_EMAIL")) {
  | Not_found => raise(Sendgrid_sender_email_not_provided)
  };

[@deriving yojson]
type from = {
  [@key "email"]
  email: string,
};

[@deriving yojson]
type contentItem = {
  [@key "type"]
  type_: string,
  [@key "value"]
  value: string,
};

[@deriving yojson]
type typeTo = {
  [@key "email"]
  email: string,
};

[@deriving yojson]
type personalizationItem = {
  [@key "to"]
  to_: list(typeTo),
};

[@deriving yojson]
type body = {
  [@key "from"]
  from,
  [@key "subject"]
  subject: string,
  [@key "content"]
  content: list(contentItem),
  [@key "personalizations"]
  personalizations: list(personalizationItem),
};

let sendEmail = (~to_, ~content, ~subject) => {
  let bodyJson =
    body_to_yojson({
      from: {
        email: senderEmail,
      },
      subject,
      content: [{type_: "text/html", value: content}],
      personalizations: [{to_: [{email: to_}]}],
    })
    |> Yojson.Safe.to_string;
  Client.post(
    ~headers=
      Cohttp.Header.(
        add_list(
          init(),
          [
            ("Authorization", "Bearer " ++ apiKey),
            ("Content-Type", "application/json"),
          ],
        )
      ),
    ~body=Cohttp_lwt__.Body.of_string(bodyJson),
    Uri.of_string("https://api.sendgrid.com/v3/mail/send"),
  );
};
