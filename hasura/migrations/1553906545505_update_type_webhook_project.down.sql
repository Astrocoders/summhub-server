ALTER TABLE projects
  DROP webhook;
ALTER TABLE projects
  add webhook varchar(255) NOT NULL default 'http://example.com/';
