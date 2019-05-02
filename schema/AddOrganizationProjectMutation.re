open Graphql_lwt;
open GraphqlHelpers;
open Library;

type addOrganizationProjectInput = {
  organizationId: string,
  name: string,
  webhook: string,
};

let addOrganizationProjectInput =
  Schema.Arg.(
    obj(
      "AddOrganizationProjectInput",
      ~fields=[
        arg("organizationId", ~typ=non_null(guid)),
        arg("name", ~typ=non_null(string)),
        arg("webhook", ~typ=non_null(string)),
      ],
      ~coerce=(organizationId, name, webhook) =>
      {organizationId, name, webhook}
    )
  );

let payload =
  Schema.(
    obj(
      "AddOrganizationProjectPayload",
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
          ~resolve=(_: Context.t, (_, project)) =>
          project
        ),
      ]
    )
  );

let addOrganizationProject =
  Schema.(
    io_field(
      "addOrganizationProject",
      ~typ=non_null(payload),
      ~args=Arg.[arg("input", ~typ=non_null(addOrganizationProjectInput))],
      ~resolve=(info, (), input) =>
      switch (info.user) {
      | Some(user) =>
        let%lwt result =
          Project.insert(
            ~name=input.name,
            ~webhook=input.webhook,
            ~organizationId=input.organizationId,
          );
        switch (result) {
        | Some(project) => Lwt.return(Ok((None, Some(project))))
        | None => Lwt.return(Ok((Some(Errors.somethingWentWrong), None)))
        };
      | None => Lwt.return(Ok((Some(Errors.unauthorized), None)))
      }
    )
  );
