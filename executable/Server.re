open Graphql_lwt;
open AppSchema;
open GraphqlHelpers;
open Library;

type timespan = {
  start: option(string),
  end_: option(string),
};

let notificationsArg =
  Schema.Arg.(
    obj(
      "Timespan",
      ~fields=[arg("start", ~typ=string), arg("end", ~typ=string)],
      ~coerce=(start, end_) =>
      {start, end_}
    )
  );

let roleResolver =
  User.(
    Schema.(
      enum(
        "Role",
        ~doc="The role of a user",
        ~values=[
          enum_value("USER", ~value=User),
          enum_value("ADMIN", ~value=Admin),
        ],
      )
    )
  );

let userResolver =
  User.(
    Schema.(
      obj("User", ~doc="A user in the system", ~fields=_ =>
        [
          field(
            "id",
            ~doc="Unique user identifier",
            ~typ=non_null(guid),
            ~args=Arg.[],
            ~resolve=(_info, p) =>
            p.id
          ),
          field(
            "name", ~args=Arg.[], ~typ=non_null(string), ~resolve=(_info, p) =>
            p.name
          ),
          field(
            "role",
            ~args=Arg.[],
            ~typ=non_null(roleResolver),
            ~resolve=(_info, p) =>
            p.role
          ),
          field(
            "summary",
            ~args=Arg.[],
            ~typ=non_null(Summary.typ),
            ~resolve=(_info, _p) =>
            {unread: 0, total: 0, projects: 0, organizations: 0}
          ),
          field(
            "notifications",
            ~args=Arg.[arg("filter", ~typ=notificationsArg)],
            ~typ=list(non_null(Notification.typ)),
            ~resolve=(_info, _p, _filter) =>
            None
          ),
          Organization.Connection.connectionResolver(
            "organizations", (_info, _p, _first, _after, _last, _before) =>
            None
          ),
        ]
      )
    )
  );

let schema =
  Graphql_lwt.Schema.(
    schema(
      [
        io_field(
          "currentUser",
          ~typ=userResolver,
          ~args=Arg.[],
          ~resolve=(info: Context.t, ()) =>
          Lwt.return(Ok(info.user))
        ),
      ],
      ~mutations=[
        SignInMutation.signIn,
        ForgotPasswordMutation.forgotPassword,
        ResetPasswordMutation.resetPassword,
        RequestSignInLinkMutation.requestSignInLink,
        SendMessageMutation.sendMessage,
        CreateOrganizationMutation.createOrganization,
        RemoveOrganizationMutation.removeOrganization,
        UpdateOrganizationNameMutation.updateOrganizationName,
        AddOrganizationMemberMutation.addOrganizationMember,
        RemoveOrganizationMemberMutation.removeOrganizationMember,
        AddOrganizationProjectMutation.addOrganizationProject,
        UpdateOrganizationProjectNameMutation.updateOrganizationProjectName,
        RemoveOrganizationProjectMutation.removeOrganizationProject,
        RecreateProjectWebhookMutation.recreateProjectWebhook,
      ],
    )
  );

module Graphql_cohttp_lwt =
  Graphql_cohttp.Make(Graphql_lwt.Schema, Cohttp_lwt.Body);

let _ =
  Lwt_main.run(
    {
      let callback =
        Graphql_cohttp_lwt.make_callback(
          req =>
            Lwt_main.run(
              {
                let connection = Database.pool;
                let%lwt user = {
                  Cohttp.Header.get(req.headers, "token")
                  |> (
                    fun
                    | None => Lwt.return(None)
                    | Some(token) => {
                        let userId = Util.decodeToken(token);
                        let%lwt user =
                          User.Model.findOne(
                            ~connection,
                            ~clause="id=" ++ "'" ++ userId ++ "'",
                            (),
                          );
                        Lwt.return(user);
                      }
                  );
                };
                Lwt.return(Context.{user, connection});
              },
            ),
          schema,
        );
      let server = Cohttp_lwt_unix.Server.make(~callback, ());
      let port = 3000;
      let mode = `TCP(`Port(port));
      Console.log(
        <Pastel>
          <Pastel> "Running at " </Pastel>
          <Pastel color=Cyan> {string_of_int(port)} </Pastel>
        </Pastel>,
      );
      Cohttp_lwt_unix.Server.create(~mode, server);
    },
  );
