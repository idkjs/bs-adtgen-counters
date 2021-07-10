# Atdgen for bucklescript, the example

A detail explanation of this example is available at [Getting started
with atdgen and
bucklescript](https://tech.ahrefs.com/getting-started-with-atdgen-and-bucklescript-1f3a14004081)

## Compile the project

```bash
esy
yarn
yarn build
```

## Run it

```
$ echo "[]" > events.json
$ node src/cli.bs.js add louis louis@nospam.com
$ node src/cli.bs.js print
=== OCaml/Reason Meetup! summary ===
date: Tue, 11 Sep 2018 15:04:13 GMT
access: public
host: louis <louis@nospam.com>
guests: 1
$ node src/cli.bs.js add bob bob@nospam.com
$ node src/cli.bs.js print
=== OCaml/Reason Meetup! summary ===
date: Tue, 11 Sep 2018 15:04:16 GMT
access: public
host: bob <bob@nospam.com>
guests: 1
=== OCaml/Reason Meetup! summary ===
date: Tue, 11 Sep 2018 15:04:13 GMT
access: public
host: louis <louis@nospam.com>
guests: 1
$ cat events.json
[
  {
    "guests": [
      {
        "email": "bob@nospam.com",
        "name": "bob"
      }
    ],
    "date": 1536678256177,
    "host": {
      "email": "bob@nospam.com",
      "name": "bob"
    },
    "name": "OCaml/Reason Meetup!",
    "access": "Public"
  },
  {
    "guests": [
      {
        "email": "louis@nospam.com",
        "name": "louis"
      }
    ],
    "date": 1536678253790,
    "host": {
      "email": "louis@nospam.com",
      "name": "louis"
    },
    "name": "OCaml/Reason Meetup!",
    "access": "Public"
  }
]
```

First add this, then `yarn --check-files`
```json
  "generators": [
    {
      "name": "atd_t",
      "command": "yarn run atdgen -t $in"
    },
    {
      "name": "atd_bs",
      "command": "yarn run atdgen -bs $in"
    }
  ],
```
then


```json
    "generators": [
      {
        "name": "atd_t",
        "edge": ["counter_t.ml", "counter_t.mli", ":", "counter.atd"]
      },
      {
        "name": "atd_bs",
        "edge": ["counter_bs.ml", "counter_bs.mli", ":", "counter.atd"]
      }
    ]
```
