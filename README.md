# SummHub Server

[![CircleCI](https://circleci.com/gh/astrocoders/summhub-server/tree/master.svg?style=svg)](https://circleci.com/gh/astrocoders/summhub-server/tree/master)


**Contains the following libraries and executables:**

```
summarize@0.0.0
│
├─test/
│   name:    TestSummarize.exe
│   main:    TestSummarize
│   require: summarize.lib
│
├─library/
│   library name: summarize.lib
│   namespace:    Summarize
│   require:
│
└─executable/
    name:    SummarizeApp.exe
    main:    SummarizeApp
    require: summarize.lib
```

## Developing:

```
npm install -g esy
git clone <this-repo>
esy install
esy build
```

## Setup Database

### Setup postgres

`
docker run -p 5432:5432 --rm postgres:latest
`

### Install Hasura CLI

https://docs.hasura.io/1.0/graphql/manual/hasura-cli/install-hasura-cli.html

### Install Hasura and get it running

`
docker run -p 8080:8080 -e HASURA_GRAPHQL_DATABASE_URL=your_database_url -e HASURA_GRAPHQL_ENABLE_CONSOLE=true -d hasura/graphql-engine:latest
`

Set your database informations following `.env.example` on `.env`

Then on the root of the project run:

`
hasura --project hasura migrate apply
`

## Running Binary:

After building the project, you can run the main binary that is produced.
```
esy x SummarizeApp.exe
```

## Running Tests:

```
# Runs the "test" command in `package.json`.
esy test
```

## Adding Migrations
`
hasura --project hasura migrate create migration_name
`
