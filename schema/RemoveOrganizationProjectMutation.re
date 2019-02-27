open Graphql_lwt;
open GraphqlHelpers;

type removeOrganizationProjectInput = {
  organizationId: string,
  projectId: string,
};

let removeOrganizationProjectInput =
  Schema.Arg.(
    obj(
      "RemoveOrganizationProjectInput",
      ~fields=[
        arg("organizationId", ~typ=non_null(guid)),
        arg("projectId", ~typ=non_null(guid)),
      ],
      ~coerce=(organizationId, projectId) =>
      {organizationId, projectId}
    )
  );

let removeOrganizationProject =
  Schema.(
    io_field(
      "removeOrganizationProject",
      ~typ=non_null(GraphqlTypes.errorPayloadType),
      ~args=
        Arg.[arg("input", ~typ=non_null(removeOrganizationProjectInput))],
      ~resolve=(info, (), _input) =>
      switch (info.user) {
      | Some(user) => Lwt.return(Ok(None))
      | None => Lwt.return(Ok(Some(Errors.unauthorized)))
      }
    )
  );
