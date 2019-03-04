open Graphql_lwt;
open GraphqlHelpers;

type recreateProjectWebhookInput = {projectId: string};

let recreateProjectWebhookInput =
  Schema.Arg.(
    obj(
      "RecreateProjectWebhookInput",
      ~fields=[arg("projectId", ~typ=non_null(guid))],
      ~coerce=projectId =>
      projectId
    )
  );

let payload =
  Schema.(
    obj(
      "RecreateProjectWebhookPayload",
      ~fields=(
                _:
                  Graphql_lwt.Schema.typ(
                    Context.t,
                    option((option(string), option(Project.t))),
                  ),
              ) =>
      [
        field(
          "error",
          ~typ=string,
          ~args=Arg.[],
          ~resolve=(_: Context.t, (error, _)) =>
          error
        ),
        field(
          "project",
          ~typ=Project.typ,
          ~args=Arg.[],
          ~resolve=(_: Context.t, (_, organization)) =>
          organization
        ),
      ]
    )
  );

let recreateProjectWebhook =
  Schema.(
    io_field(
      "recreateProjectWebhook",
      ~typ=non_null(payload),
      ~args=Arg.[arg("input", ~typ=non_null(recreateProjectWebhookInput))],
      ~resolve=(info, (), _input) =>
      switch (info.user) {
      | Some(user) => Lwt.return(Ok((None, None)))
      | None => Lwt.return(Ok((Some(Errors.unauthorized), None)))
      }
    )
  );
