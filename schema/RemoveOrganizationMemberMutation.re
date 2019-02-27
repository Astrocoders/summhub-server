open Graphql_lwt;
open GraphqlHelpers;

type removeOrganizationMemberInput = {
  organizationId: string,
  memberId: string,
};

let removeOrganizationMemberInput =
  Schema.Arg.(
    obj(
      "RemoveOrganizationMemberInput",
      ~fields=[
        arg("organizationId", ~typ=non_null(guid)),
        arg("memberId", ~typ=non_null(guid)),
      ],
      ~coerce=(organizationId, memberId) =>
      {organizationId, memberId}
    )
  );

let removeOrganizationMember =
  Schema.(
    io_field(
      "removeOrganizationMember",
      ~typ=non_null(GraphqlTypes.errorPayloadType),
      ~args=
        Arg.[arg("input", ~typ=non_null(removeOrganizationMemberInput))],
      ~resolve=(info, (), _input) =>
      switch (info.user) {
      | Some(user) => Lwt.return(Ok(None))
      | None => Lwt.return(Ok(Some(Errors.unauthorized)))
      }
    )
  );
