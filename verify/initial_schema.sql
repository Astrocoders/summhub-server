BEGIN;

SELECT pg_catalog.has_schema_privilege('initial_schema', 'usage');

ROLLBACK;
