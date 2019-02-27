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

let removeOrganizationMember =
  Schema.(
    io_field(
      "removeOrganizationMember",
      ~typ=non_null(payload),
      ~args=
        Arg.[arg("input", ~typ=non_null(removeOrganizationMemberInput))],
      ~resolve=(info, (), _input) =>
      switch (info.user) {
      | Some(user) => Lwt.return(Ok(None))
      | None => Lwt.return(Ok(Some(Errors.unauthorized)))
      }
    )
  );
