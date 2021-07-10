[@bs.module "uuid"] external uuid: unit => string = "v4";
let event_file = "counters.json";
// module Counter_t = Counters.Counter_t;

let read_counter = () => {
  /* read the file from disk */
  let file_content = Node.Fs.readFileAsUtf8Sync(event_file);
  /* parse the json */
  let json = Js.Json.parseExn(file_content);
  /* turn it into a proper record */
  let counter: Counter_t.counter = (
    Atdgen_codec_runtime.Decode.decode(Counter_bs.read_counter, json): Counter_t.counter
  );
  counter;
};
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

let () =
  switch (Array.to_list(Sys.argv)) {
  | [_, _, "print", ..._] => print_counters()
  | [_, _, "add", name, count, ..._] => add_counter(name, count)

  | _ => print_endline("usage: nodejs cli.bs.js <print|add>")
  };
