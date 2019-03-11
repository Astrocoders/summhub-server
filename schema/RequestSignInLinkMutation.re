open Graphql_lwt;
open Cohttp_lwt_unix;
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
      ~resolve=(context: Context.t, (), email) => {
        let%lwt user = User.getByEmail(context.connection, email);
        switch (user) {
        | Some(user) =>
          let%lwt (response, _) =
            Sendgrid.sendEmail(
              ~to_=email,
              ~subject="Summhub - Login Access Link",
              ~content=
                "<a href=\"https://summhub.com/sign-in/\""
                ++ Auth.encodeToken(user.id)
                ++ " > Click here to sign in </a>",
            );
          /* Sendgrid uses the code 202 to indicate success otherwise is fail https://sendgrid.com/docs/API_Reference/api_v3.html */
          switch (response.status) {
          | `Code(202) => Lwt.return(Ok(None))
          | _ => Lwt.return(Ok(Some(Errors.errorOnEmailSend)))
          };
          Lwt.return(Ok(None));
        | None =>
          let%lwt result = User.createUser(context.connection, email);
          switch (result) {
          | None => Lwt.return(Ok(Some(Errors.somethingWentWrong)))
          | Some(_) =>
            let%lwt user = User.getByEmail(context.connection, email);
            switch (user) {
            | Some(user) =>
              let%lwt (response, _) =
                Sendgrid.sendEmail(
                  ~to_=email,
                  ~subject="Summhub - Login Access Link",
                  ~content=
                    "<a href=\"https://summhub.com/sign-in/\""
                    ++ Auth.encodeToken(user.id)
                    ++ " > Click here to sign in </a>",
                );
              switch (response.status) {
              | `Code(202) => Lwt.return(Ok(None))
              | _ => Lwt.return(Ok(Some(Errors.errorOnEmailSend)))
              };
            | None => Lwt.return(Ok(Some(Errors.somethingWentWrong)))
            };
          };
        };
      },
    )
  );
