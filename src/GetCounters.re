let url = "http://127.0.0.1:3003/counters";

let raiseOnNok = (promise: Js.Promise.t(Fetch.Response.t)) => {
  promise
  |> Js.Promise.then_(r => {
       Js.log2("Called raiseOnNok", r);
       r |> Fetch.Response.ok
         ? promise : Js.Exn.raiseError(Fetch.Response.statusText(r));
     });
};

let promiseToOptionalJson =
    (promise: Js.Promise.t(Fetch.response))
    : Js.Promise.t(option(Js.Json.t)) => {
  promise
  |> raiseOnNok
  |> Js.Promise.then_(Fetch.Response.json)
  |> Js.Promise.then_(v => {
       let v = v->Some->Js.Promise.resolve;
       Js.log2("Called promiseToOptionalJson", v);
       v;
     })
  |> Js.Promise.catch(e => {
       Js.log2("Unexpected error: ", e);
       None->Js.Promise.resolve;
     });
};

// let fetch = (url: string): Js.Promise.t(option(Js.Json.t)) => {
//   // let json = Js.Json.parseExn(url)|>Obj.magic|>promiseToOptionalJson;
//   // let json = Js.Json.parseExn(url);
//   Js.log("Called");

//   let x = Fetch.fetch(url) |> promiseToOptionalJson;
//   Js.log(x)->Js.Promise.resolve->ignore;
//   Fetch.fetch(url) |> promiseToOptionalJson;
// };
// Js.log(fetch(url));
let authenticate = (res: Fetch.Response.t):Js.Promise.t(Js.Json.t) => {
  let status = Fetch.Response.status(res);
  switch (status) {
  | 200 => Fetch.Response.json(res)
  | 401 =>
    Js.log("/login");
    Js.Exn.raiseError(Fetch.Response.statusText(res));
  | _ => Js.Exn.raiseError(Fetch.Response.statusText(res))
  };
};
let _ =
    Js.Promise.(
      Fetch.fetch(url)
      |> then_(authenticate)
      |> then_(json => json |> HandleCounterData.handleCounterData  |> resolve)
    );

