ALTER TABLE projects
  DROP webhook;
ALTER TABLE projects
  add webhook uuid NOT NULL default uuid_generate_v4();
