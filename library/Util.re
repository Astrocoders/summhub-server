let foo = () => print_endline("Hello");

module JSON = {
  let getStringWithDefault = (default, json) => {
    switch (json) {
    | `String(value) => value
    | _ => default
    };
  };
};
