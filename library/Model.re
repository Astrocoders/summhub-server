module type Config = {
  type t;
  let table: string;
  let parseRow: array(string) => t;
};

module Make = (Config: Config) => {
  open Ezpostgresql.Pool;
  open Lwt_result.Infix;

  let create = (~connection, ~fields, ~values, ()) => {
    let query =
      "insert into "
      ++ Config.table
      ++ " ("
      ++ (fields |> String.concat(","))
      ++ ")"
      ++ " values ("
      ++ (values |> String.concat(","))
      ++ ")";
    let%lwt operation_result = command(~query, connection);

    switch (operation_result) {
    | Ok(_) => Lwt.return(Some("success"))
    | _ => Lwt.return(None)
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
    let%lwt operation_result =
      one(~query, connection)
      >>= (
        row_opt =>
          switch (row_opt) {
          | Some(row) => Lwt_result.return(Some(Config.parseRow(row)))
          | None => Lwt_result.return(None)
          }
      );

    switch (operation_result) {
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
    let%lwt operation_result =
      all(~query, connection)
      >>= (
        result =>
          Lwt_result.return(
            Array.map(item => Config.parseRow(item), result),
          )
      );

    switch (operation_result) {
    | Ok(parsedItems) => Lwt.return(Some(parsedItems))
    | _ => Lwt.return(None)
    };
  };
};
