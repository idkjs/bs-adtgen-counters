let event_file = "counters.json";
// module Counter_t = Counters.Counter_t;
let print_counter = (counter: Counter_t.t) => {
  Js.log2("=== %s summary ===", counter.name);
  Js.log2("value: %s", counter.value);
  // Js.log2(
  //   "access: %s",
  //   switch (event.access) {
  //   | `Private(_) => "private (registration required)"
  //   | `Public({address}) => "public (location: `" ++ address ++ "`)"
  //   },
  // );
  // Js.log4(
  //   "host: %s <%s>%s",
  //   event.host.name,
  //   event.host.email,
  //   switch (event.host.phone) {
  //   | None => ""
  //   | Some(p) => " (" ++ p ++ ")"
  //   },
  // );
  // Js.log2("guests: %d", List.length(counters));
};

let read_counters = () => {
  /* read the file from disk */
  let file_content = Node.Fs.readFileAsUtf8Sync(event_file);
  /* parse the json */
  let json = Js.Json.parseExn(file_content);
  /* turn it into a proper record */
  let counters: Counter_t.counters = (
    Atdgen_codec_runtime.Decode.decode(Counter_bs.read_counters, json): Counter_t.counters
  );
  counters;
};

let write_counters = counters => {
  /* turn a list of records into json */
  let json =
    Atdgen_codec_runtime.Encode.encode(Counter_bs.write_counters, counters);
  /* convert the json to string */
  let file_content = Js.Json.stringifyWithSpace(json, 2);
  /* write the json in our file */
  Node.Fs.writeFileAsUtf8Sync(event_file, file_content);
};
let ctrs = read_counters()
let print_counters = () =>  List.iter(c =>print_counter(c),ctrs.counters);

// let add_counter = (name, email) => {
//   let events = read_counters();
//   let host = {Counter_t.name, email, phone: None};

//   let new_event = {
//     Counter_t.access: `Public({address: "Central Park"}),
//     name: "OCaml/Reason Meetup!",
//     host,
//     date: Js.Date.make(),
//     guests: [host],
//   };

//   write_counters([new_event, ...counters]);
// };

let () =
  switch (Array.to_list(Sys.argv)) {
  | [_, _, "print", ..._] => print_counters()
  // | [_, _, "add", name, email, ..._] => add_event(name, email)
  | _ => print_endline("usage: nodejs cli.bs.js <print|add>")
  };
