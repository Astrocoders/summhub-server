let foo = () => print_endline("Hello");

module Option = {
  let getWithDefault = (default, wrappedValue) => {
    switch (wrappedValue) {
    | Some(value) => value
    | None => default
    };
  };
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
