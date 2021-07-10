type t('a) =
  | NotFired
  | Loading
  | Failure(string)
  | Success('a);

let mapSuccess = (fn, t) => {
  switch (t) {
  | Success(a) => fn(Some(a))
  | _ => fn(None)
  };
};

let fromPromiseError = (err: Js.Promise.error): string =>
  Js.Json.stringify(Obj.magic(err));
