[@bs.module "uuid"] external uuid: unit => string = "v4";

[@bs.module]
external isomorphicFetch: string => Js.Promise.t('a) = "isomorphic-fetch";
let event_file = "counters.json";
// module Counter_t = Counters.Counter_t;
let endpoint = "http://127.0.0.1:3003/counters";

let read_counters = () => {
  /* read the file from disk */
  let file_content = Node.Fs.readFileAsUtf8Sync(event_file);
  /* parse the json */
  let json = Js.Json.parseExn(file_content);
  /* turn it into a proper record */
  let counters: Counter_t.counters =
    Atdgen_codec_runtime.Decode.decode(Counter_bs.read_counters, json);
  counters;
};
let logfake = data => {
  let fake = Obj.magic(data);
  Js.log2("handleFakeCounterData:", fake);
};
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

let write_counters = counters => {
  /* turn a list of records into json */
  let json =
    Atdgen_codec_runtime.Encode.encode(Counter_bs.write_counters, counters);
  /* convert the json to string */
  Js.log2("json: ", "json");
  Js.log2("json: ", json);
  Js.log2("counters: ", counters);
  let file_content = Js.Json.stringifyWithSpace(json, 2);
  /* write the json in our file */
  Node.Fs.writeFileAsUtf8Sync(event_file, file_content);
};
let ctrs: Counter_t.counters = read_counters();

let add_counter = (name, count) => {
  let count = count->int_of_string;
  let new_counter: Counter_t.counter = {
    id: Some(uuid()),
    name,
    value: count,
  };
  let new_counters: Counter_t.counters = {
    counters: List.append(ctrs.counters, [new_counter]),
  };
  write_counters(new_counters);
};

let print_counters = () => {
  let print_counter = (counter: Counter_t.counter) => {
    Js.log2("=== %s summary ===", counter.name);
    Js.log2("value: %s", counter.value);
  };
  List.iter(c => print_counter(c), ctrs.counters);
  Js.log2("# of counters is: %d", List.length(ctrs.counters));
};

// let () =
//   switch (Array.to_list(Sys.argv)) {
//   | [_, _, "print", ..._] => print_counters()
//   | [_, _, "add", name, count, ..._] => add_counter(name, count)

//   | _ => print_endline("usage: nodejs cli.bs.js <print|add>")
//   };

let read_counters = json => {
  /* read the file from disk */
  // let file_content = Node.Fs.readFileAsUtf8Sync(event_file);
  /* parse the json */
  let json = Js.Json.parseExn(json);
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
