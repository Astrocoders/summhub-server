// Update with your config settings.

module.exports = {

  development: {
    client: 'postgresql',
    connection: {
      host: process.env.PGHOST || '127.0.0.1',
      database: process.env.PGDATABASE ||'summhub_dev',
      user:     process.env.PGUSER,
      password: process.env.PGPASSWORD
    },
    pool: {
      min: 2,
      max: 10
    },
    migrations: {
      tableName: 'knex_migrations'
    }
  },

  production: {
    client: 'postgresql',
    connection: {
      host: process.env.PGHOST,
      database: process.env.PGDATABASE ||'summhub_prod',
      user:     process.env.PGUSER,
      password: process.env.PGPASSWORD
    },
    pool: {
      min: 2,
      max: 10
    },
    migrations: {
      tableName: 'knex_migrations'
    }
  }

};
