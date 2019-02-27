open Graphql_lwt;
open GraphqlHelpers;

type updateOrganizationNameInput = {
  organizationId: int,
  name: string,
};

let updateOrganizationNameInput =
  Schema.Arg.(
    obj(
      "UpdateOrganizationNameInput",
      ~fields=[
        arg("organizationId", ~typ=non_null(int)),
        arg("name", ~typ=non_null(string)),
      ],
      ~coerce=(organizationId, name) =>
      {organizationId, name}
    )
  );

let payload =
  Schema.(
    obj(
      "UpdateOrganizationNamePayload",
      ~fields=(
                _:
                  Graphql_lwt.Schema.typ(
                    Context.t,
                    option((option(string), option(Organization.t))),
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
          "organization",
          ~typ=Organization.resolver,
          ~args=Arg.[],
          ~resolve=(_: Context.t, (_, organization)) =>
          organization
        ),
      ]
    )
  );

let updateOrganizationName =
  Schema.(
    io_field(
      "updateOrganizationName",
      ~typ=non_null(payload),
      ~args=Arg.[arg("input", ~typ=non_null(updateOrganizationNameInput))],
      ~resolve=(info, (), _input) =>
      switch (info.user) {
      | Some(user) => Lwt.return(Ok((None, None)))
      | None => Lwt.return(Ok((Some(Errors.unauthorized), None)))
      }
    )
  );
