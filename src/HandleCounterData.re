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
