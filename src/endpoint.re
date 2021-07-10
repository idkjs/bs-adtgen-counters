let endpoint = "http://127.0.0.1:3003/counters";
[@bs.module]
external isomorphicFetch: string => Js.Promise.t('a) = "isomorphic-fetch";
let handleCounterData = data => {
  /* read the file from disk */
  // let file_content = Node.Fs.readFileAsUtf8Sync(event_file);
  /* parse the json */
  // logfake(data)->ignore;
  Js.log2("handleCounterData:", data);

  // let json = Js.Json.parseExn(data);
  let dict = Js.Dict.empty();
  Js.Dict.set(dict, "counters", data);
  let json: Js.Json.t = Js.Json.object_(dict);
  /* turn it into a proper record */
  let counters: Counter_t.counters =
    Atdgen_codec_runtime.Decode.decode(Counter_bs.read_counters, json);
  counters;
};

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
       //  Js.log(Decoder.read_counters((v|>Belt.Option.getExn)))
       Js.log2("Called promiseToOptionalJson", v);
       v;
     })
  |> Js.Promise.catch(e => {
       Js.log2("Unexpected error: ", e);
       None->Js.Promise.resolve;
     });
};
Js.Promise.(
  isomorphicFetch(endpoint)
  |> then_(response => response##json())
  |> then_(data => data |> handleCounterData |> resolve)
);
// let fetch = (url: string): Js.Promise.t(option(Js.Json.t)) => {
//   // let json = Js.Json.parseExn(url)|>Obj.magic|>promiseToOptionalJson;
//   // let json = Js.Json.parseExn(url);
//   Js.log("Called");

//   let x = Fetch.fetch(url) |> promiseToOptionalJson;
//   Js.log(x)->Js.Promise.resolve->ignore;
//   Fetch.fetch(url) |> promiseToOptionalJson;
// };
// Js.log(fetch(endpoint));
