let event_file = "counters.json";

let print_event = (counter: Counters_t.t) => {
  Js.log2("=== %s summary ===", counter.name);
  Js.log2("date: %s", counter.id);
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
  Js.log2("guests: %d", List.length(counters));
};

let read_counters = () => {
  /* read the file from disk */
  let file_content = Node.Fs.readFileAsUtf8Sync(event_file);
  /* parse the json */
  let json = Js.Json.parseExn(file_content);
  /* turn it into a proper record */
  let events: Counters_t.counters = (
    Atdgen_codec_runtime.Decode.decode(Counters_bs.read_counters, json): Counters_t.counters
  );
  events;
};

let write_counters = events => {
  /* turn a list of records into json */
  let json =
    Atdgen_codec_runtime.Encode.encode(Counters_bs.write_counters, events);
  /* convert the json to string */
  let file_content = Js.Json.stringifyWithSpace(json, 2);
  /* write the json in our file */
  Node.Fs.writeFileAsUtf8Sync(event_file, file_content);
};

let print_counters = () => read_counters() |> List.iter(print_event);

// let add_counter = (name, email) => {
//   let events = read_counters();
//   let host = {Counters_t.name, email, phone: None};

//   let new_event = {
//     Counters_t.access: `Public({address: "Central Park"}),
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
