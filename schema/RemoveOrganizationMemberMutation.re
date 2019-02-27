open Graphql_lwt;
open GraphqlHelpers;

type removeOrganizationMemberInput = {
  organizationId: int,
  memberId: int,
};

let removeOrganizationMemberInput =
  Schema.Arg.(
    obj(
      "RemoveOrganizationMemberInput",
      ~fields=[
        arg("organizationId", ~typ=non_null(int)),
        arg("memberId", ~typ=non_null(int)),
      ],
      ~coerce=(organizationId, memberId) =>
      {organizationId, memberId}
    )
  );

let removeOrganizationMember =
  Schema.(
    io_field(
      "removeOrganizationMember",
      ~typ=non_null(GraphqlTypes.errorPayload),
      ~args=
        Arg.[arg("input", ~typ=non_null(removeOrganizationMemberInput))],
      ~resolve=(info, (), _input) =>
      switch (info.user) {
      | Some(user) => Lwt.return(Ok(None))
      | None => Lwt.return(Ok(Some(Errors.unauthorized)))
      }
    )
  );
