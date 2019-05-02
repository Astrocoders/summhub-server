open Graphql_lwt;
open GraphqlHelpers;
open Library;

type addOrganizationMemberInput = {
  organizationId: string,
  email: string,
};

let addOrganizationMemberInput =
  Schema.Arg.(
    obj(
      "AddOrganizationMemberInput",
      ~fields=[
        arg("organizationId", ~typ=non_null(guid)),
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
                    option((option(string), option(Member.t))),
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
          ~typ=Member.typ,
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
      ~resolve=(info, (), input) =>
      switch (info.user) {
      | Some(user) =>
        let%lwt result =
          Member.insert(
            ~email=input.email,
            ~organizationId=input.organizationId,
            (),
          );
        switch (result) {
        | Some(member) => Lwt.return(Ok((None, Some(member))))
        | None => Lwt.return(Ok((Some(Errors.somethingWentWrong), None)))
        };
      | None => Lwt.return(Ok((Some(Errors.unauthorized), None)))
      }
    )
  );
