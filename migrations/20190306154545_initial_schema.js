exports.up = function(knex, Promise) {
  return Promise.all([
    knex.schema.createTable('app_users', table => {
      table
        .uuid('id')
        .notNullable()
        .primary()
      table.string('email').notNullable()
      table
        .enu('role', ['ADMIN', 'USER'])
        .defaultTo('USER')
        .notNullable()
      table.timestamp('created_at').defaultTo(knex.fn.now())
    }),
    knex.schema.createTable('organizations', table => {
      table
        .uuid('id')
        .notNullable()
        .primary()
      table.string('name').notNullable()
      table.timestamp('created_at').defaultTo(knex.fn.now())
      table.uuid('user_id').notNullable().references('id').inTable('app_users')
    }),
    knex.schema.createTable('members', table => {
      table
        .uuid('id')
        .notNullable()
        .primary()
      table.uuid('organization_id').notNullable().references('id').inTable('organizations')
      table.string('email').notNullable()
      table.timestamp('created_at').defaultTo(knex.fn.now())
    }),
    knex.schema.createTable('projects', table => {
      table
        .uuid('id')
        .notNullable()
        .primary()
      table.uuid('organization_id').notNullable().references('id').inTable('organizations')
      table.string('name').notNullable()
      table.string('webhook').notNullable()
      table.timestamp('created_at').defaultTo(knex.fn.now())
    }),
    knex.schema.createTable('notifications', table => {
      table
        .uuid('id')
        .notNullable()
        .primary()
      table.uuid('user_id').notNullable().references('id').inTable('app_users')
      table.string('title').notNullable()
      table.string('body').notNullable()
      table.string('icon')
      table.string('link')
      table.string('payload').notNullable()
      table.timestamp('created_at').defaultTo(knex.fn.now())
    }),
    knex.schema.createTable('messages', table => {
      table
        .uuid('id')
        .notNullable()
        .primary()
      table.uuid('notification_id').notNullable().references('id').inTable('notifications')
      table.string('email').notNullable()
      table.string('message').notNullable()
      table.timestamp('created_at').defaultTo(knex.fn.now())
    }),
  ])
}

exports.down = function(knex, Promise) {
  return Promise.all([
    knex.schema.dropTableIfExists('members'),
    knex.schema.dropTableIfExists('projects'),
    knex.schema.dropTableIfExists('organizations'),
    knex.schema.dropTableIfExists('messages'),
    knex.schema.dropTableIfExists('notifications'),
    knex.schema.dropTableIfExists('app_users'),
  ])
}
