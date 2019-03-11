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

Set your database informations following `.env.example` on `.env`

Then on the root of the project run:

`yarn knex:migrate`


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

`yarn knex:create`
