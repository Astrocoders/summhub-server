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
          let%lwt (response, _) = User.sendSignInLink(user);
          /* Sendgrid uses the code 202 to indicate success otherwise is fail https://sendgrid.com/docs/API_Reference/api_v3.html */
          switch (response.status) {
          | `Code(202) => Lwt.return(Ok(None))
          | `Accepted => Lwt.return(Ok(None))
          | _ => Lwt.return(Ok(Some(Errors.errorOnEmailSend)))
          };
          Lwt.return(Ok(None));
        | None =>
          let%lwt result = User.insert(context.connection, ~email, ~role="USER");
          switch (result) {
          | None => Lwt.return(Ok(Some(Errors.somethingWentWrong)))
          | Some(user) =>
            let%lwt (response, _) = User.sendSignInLink(user);
            switch (response.status) {
            | `Code(202) => Lwt.return(Ok(None))
            | `Accepted => Lwt.return(Ok(None))
            | _ => Lwt.return(Ok(Some(Errors.errorOnEmailSend)))
            };
          };
        };
      },
    )
  );
