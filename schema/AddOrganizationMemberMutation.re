open Graphql_lwt;

type addOrganizationMemberInput = {
  organizationId: int,
  email: string,
};

let addOrganizationMemberInput =
  Schema.Arg.(
    obj(
      "AddOrganizationMemberInput",
      ~fields=[
        arg("organizationId", ~typ=non_null(int)),
        arg("email", ~typ=non_null(string)),
      ],
      ~coerce=(organizationId, email) =>
      {organizationId, email}
    )
  );

let payload =
  Schema.(
    obj(
      "AddOrganizationMemberPayload",
      ~fields=(
                _:
                  Graphql_lwt.Schema.typ(
                    Context.t,
                    option(
                      (option(string), option(Organization.Member.t)),
                    ),
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
          "member",
          ~typ=Organization.Member.resolver,
          ~args=Arg.[],
          ~resolve=(_: Context.t, (_, member)) =>
          member
        ),
      ]
    )
  );

let addOrganizationMember =
  Schema.(
    io_field(
      "addOrganizationMember",
      ~typ=non_null(payload),
      ~args=Arg.[arg("input", ~typ=non_null(addOrganizationMemberInput))],
      ~resolve=(info, (), _input) =>
      switch (info.user) {
      | Some(user) => Lwt.return(Ok((None, None)))
      | None => Lwt.return(Ok((Some("User not authenticated"), None)))
      }
    )
  );