open Graphql_lwt;

type requestSignInLinkInput = {email: string};

let requestSignInLinkInput =
  Schema.Arg.(
    obj(
      "RequestSignInLinkInput",
      ~fields=[arg("email", ~typ=non_null(string))],
      ~coerce=email =>
      {email}
    )
  );

let requestSignInLink =
  Schema.(
    io_field(
      "requestSignInLink",
      ~typ=non_null(GraphqlTypes.errorPayload),
      ~args=Arg.[arg("input", ~typ=non_null(requestSignInLinkInput))],
      ~resolve=(_, (), input) =>
      Lwt.return(Ok(None))
    )
  );
