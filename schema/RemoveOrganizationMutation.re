open Graphql_lwt;
open GraphqlHelpers;

type removeOrganizationInput = {organizationId: int};

let removeOrganizationInput =
  Schema.Arg.(
    obj(
      "RemoveOrganizationInput",
      ~fields=[arg("organizationId", ~typ=non_null(int))],
      ~coerce=organizationId =>
      organizationId
    )
  );

let removeOrganization =
  Schema.(
    io_field(
      "removeOrganization",
      ~typ=non_null(GraphqlTypes.errorPayload),
      ~args=Arg.[arg("input", ~typ=non_null(removeOrganizationInput))],
      ~resolve=(info, (), _input) =>
      switch (info.user) {
      | Some(user) => Lwt.return(Ok(None))
      | None => Lwt.return(Ok(Some(Errors.unauthorized)))
      }
    )
  );
