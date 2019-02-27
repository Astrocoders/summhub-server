open Graphql_lwt;
open GraphqlHelpers;

type removeOrganizationProjectInput = {
  organizationId: int,
  projectId: int,
};

let removeOrganizationProjectInput =
  Schema.Arg.(
    obj(
      "RemoveOrganizationProjectInput",
      ~fields=[
        arg("organizationId", ~typ=non_null(int)),
        arg("projectId", ~typ=non_null(int)),
      ],
      ~coerce=(organizationId, projectId) =>
      {organizationId, projectId}
    )
  );

let payload =
  Schema.(
    obj(
      "ErrorPayload",
      ~fields=(
                _: Graphql_lwt.Schema.typ(Context.t, option(option(string))),
              ) =>
      [
        field(
          "error", ~typ=string, ~args=Arg.[], ~resolve=(_: Context.t, error) =>
          error
        ),
      ]
    )
  );

let removeOrganizationProject =
  Schema.(
    io_field(
      "removeOrganizationProject",
      ~typ=non_null(payload),
      ~args=
        Arg.[arg("input", ~typ=non_null(removeOrganizationProjectInput))],
      ~resolve=(info, (), _input) =>
      switch (info.user) {
      | Some(user) => Lwt.return(Ok(None))
      | None => Lwt.return(Ok(Some(Errors.unauthorized)))
      }
    )
  );
