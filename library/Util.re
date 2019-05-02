let foo = () => print_endline("Hello");

module Option = {
  let getWithDefault = (default, wrappedValue) => {
    switch (wrappedValue) {
    | Some(value) => value
    | None => default
    };
  };
};

module Calendar = {
  open CalendarLib;
  let defaultToNow = value => Option.getWithDefault(Calendar.now(), value);
  let toDateString = calendar =>
    Calendar.(calendar |> to_date |> Printer.Date.to_string);
};

module JSON = {
  let getStringWithDefault = (default, json) => {
    switch (json) {
    | `String(value) => value
    | _ => default
    };
  };
  let encodeCreatedAt = createdAt =>
    Option.getWithDefault(`Assoc([("created_at", `String(""))]), createdAt)
    |> getStringWithDefault("");
};
