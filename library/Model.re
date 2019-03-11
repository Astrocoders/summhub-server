module type Config = {
  type t;
  let table: string;
  let parseRow: array(string) => t;
};

module Make = (Config: Config) => {
  open Ezpostgresql.Pool;
  open Lwt_result.Infix;

  exception Error_on_database_insert;

  let insert = (~connection, ~fields: list((string, string)), ()) => {
    let query =
      "insert into "
      ++ Config.table
      ++ " ("
      ++ (fields |> List.map(((name, _)) => name) |> String.concat(","))
      ++ ")"
      ++ " values ("
      ++ (fields |> List.map(((_, value)) => value) |> String.concat(","))
      ++ ")";
    let%lwt operationResult = command(~query, connection);

    switch (operationResult) {
    | Ok(response) => Lwt.return(Some(response))
    | _ => Lwt.fail(Error_on_database_insert)
    };
  };

  let findOne = (~connection, ~clause=?, ()) => {
    let query =
      "select * from "
      ++ Config.table
      ++ (
        switch (clause) {
        | Some(clause) => " where " ++ clause
        | None => ""
        }
      );
    let%lwt operationResult =
      one(~query, connection)
      >>= (
        rowOpt =>
          switch (rowOpt) {
          | Some(row) => Lwt_result.return(Some(Config.parseRow(row)))
          | None => Lwt_result.return(None)
          }
      );

    switch (operationResult) {
    | Ok(Some(parsedRow)) => Lwt.return(Some(parsedRow))
    | _ => Lwt.return(None)
    };
  };

  let find = (~connection, ~clause=?, ()) => {
    let query =
      "select * from "
      ++ Config.table
      ++ (
        switch (clause) {
        | Some(clause) => " where " ++ clause
        | None => ""
        }
      );
    let%lwt operationResult =
      all(~query, connection)
      >>= (
        result =>
          Lwt_result.return(
            Array.map(item => Config.parseRow(item), result),
          )
      );

    switch (operationResult) {
    | Ok(parsedItems) => Lwt.return(Some(parsedItems))
    | _ => Lwt.return(None)
    };
  };
};
