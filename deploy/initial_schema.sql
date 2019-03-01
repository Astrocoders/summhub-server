BEGIN;

create table app_users
(
  id   uuid default uuid_generate_v4() not null
    constraint app_users_pkey
      primary key,
  name varchar(150)                    not null,
  role varchar(10)                     not null
);

create table organizations
(
  id         uuid      default uuid_generate_v4() not null
    constraint organizations_pkey
      primary key,
  name       varchar(200),
  created_at timestamp default now(),
  user_id    uuid                                 not null
    constraint organizations_user_id_fkey
      references app_users
);

create table members
(
  id              uuid      default uuid_generate_v4() not null
    constraint members_pkey
      primary key,
  organization_id uuid
    constraint members_organization_id_fkey
      references organizations,
  email           varchar(200)                         not null,
  created_at      timestamp default now()
);

create table projects
(
  id              uuid      default uuid_generate_v4() not null
    constraint projects_pkey
      primary key,
  organization_id uuid                                 not null
    constraint projects_organization_id_fkey
      references organizations,
  name            varchar(200)                         not null,
  webhook         text                                 not null,
  created_at      timestamp default now()
);

create table notifications
(
  id         uuid      default uuid_generate_v4() not null
    constraint notifications_pkey
      primary key,
  user_id    uuid                                 not null
    constraint notifications_user_id_fkey
      references app_users,
  title      varchar(200)                         not null,
  body       text                                 not null,
  icon       text,
  link       text,
  payload    varchar(350)                         not null,
  created_at timestamp default now()
);

create table messages
(
  id              uuid      default uuid_generate_v4() not null
    constraint messages_pkey
      primary key,
  notification_id uuid                                 not null
    constraint messages_notification_id_fkey
      references notifications,
  email           varchar(250)                         not null,
  message         varchar(300)                         not null,
  created_at      timestamp default now()
);

COMMIT;
