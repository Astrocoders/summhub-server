open Graphql_lwt;
open GraphqlHelpers;
open Library;

type requestSignInLinkInput = {email: string};

let requestSignInLinkInput =
  Schema.Arg.(
    obj(
      "RequestSignInLinkInput",
      ~fields=[arg("email", ~typ=non_null(string))],
      ~coerce=email =>
      email
    )
  );

let requestSignInLink =
  Schema.(
    io_field(
      "requestSignInLink",
      ~typ=non_null(GraphqlTypes.errorPayloadType),
      ~args=Arg.[arg("input", ~typ=non_null(requestSignInLinkInput))],
      ~resolve=(context: Context.t, (), email) =>
      switch (User.getByEmail(context.connection, email)) {
      | Some(user) =>
        Email.sendEmail(
          ~from="contact@summhub.com",
          ~to_=email,
          ~subject="Summhub - Login Access Link",
          ~content=
            "Your access link is summhub://access-link/"
            ++ Auth.encodeToken(user.id),
        )
        |> ignore;
        Lwt.return(Ok(None));
      | None =>
        let result = User.createUser(context.connection, email);
        switch (result) {
        | None => Lwt.return(Ok(Some(Errors.somethingWentWrong)))
        | Some(_) =>
          switch (User.getByEmail(context.connection, email)) {
          | Some(user) =>
            Email.sendEmail(
              ~from="contact@summhub.com",
              ~to_=email,
              ~subject="Summhub - Login Access Link",
              ~content=
                "Your access link is summhub://access-link/"
                ++ Auth.encodeToken(user.id),
            )
            |> ignore;
            Lwt.return(Ok(None));
          | None => Lwt.return(Ok(Some(Errors.somethingWentWrong)))
          }
        };
      }
    )
  );
