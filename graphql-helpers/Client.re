module Error = {
  type t = unit;
};

exception Hasura_not_configured;

let wrapStringValue = value => "'" ++ value ++ "'";

let hasuraUrl =
  try (Sys.getenv("HASURA_URL")) {
  | Not_found => raise(Hasura_not_configured)
  };

let readResponse = responseBody => {
  let (readerPromise, onFinish) = Lwt.wait();
  let rec readResponse = prevStrings => {
    Httpaf.Body.(
      schedule_read(
        responseBody,
        ~on_eof=
          () => {
            let string = String.concat("", prevStrings |> List.rev);
            switch (Yojson.Safe.from_string(string) |> Yojson.Safe.to_basic) {
            | exception x => Lwt.wakeup_exn(onFinish, x)
            | json =>
              switch (Yojson.Basic.Util.member("data", json)) {
              | `Null => Lwt.wakeup_exn(onFinish, Invalid_argument(string))
              | data => Lwt.wakeup_later(onFinish, data)
              }
            };
          },
        ~on_read=
          (fragment, ~off, ~len) => {
            let nextFragmentString = Bytes.create(len);
            Lwt_bytes.blit_to_bytes(
              fragment,
              off,
              nextFragmentString,
              0,
              len,
            );
            readResponse([
              Bytes.to_string(nextFragmentString),
              ...prevStrings,
            ]);
          },
      )
    );
  };
  readResponse([]);
  readerPromise |> Lwt_result.catch;
};

[@deriving yojson]
type queryBody = {
  [@key "query"]
  query: string,
  [@key "variables"]
  variables: Yojson.Safe.t,
};

let get = (query, ~variables, parse) => {
  open Httpkit_client;
  let uri = Uri.of_string(hasuraUrl);
  let requestBody =
    queryBody_to_yojson({query, variables}) |> Yojson.Safe.to_string;
  let request = Request.create(~body=requestBody, `POST, uri);
  Httpkit_lwt_client.(
    Lwt.Infix.(
      Http.send(request)
      >>= (
        fun
        | Ok((_, body)) => {
            readResponse(body)
            |> Lwt_result.map(x =>
                 switch (parse(x)) {
                 | exception x =>
                   Printexc.to_string(x) |> print_endline;
                   raise(x);
                 | x => x
                 }
               )
            |> Lwt_result.map_err(exc =>
                 Printexc.to_string(exc) |> print_endline
               );
          }
        | Error(_) => {
            Lwt_result.fail();
          }
      )
    )
  );
};
