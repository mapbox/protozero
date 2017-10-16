
# Protobuf decoder tool

Tool to decode unknown protocol buffer encoded messages. The protocol buffer
format doesn't contain enough information about the contents of a file to make
it decodable without the format description usually found in a `.proto` file,
so this tool does some informed guessing.

Reads from STDIN and writes to STDOUT.

To execute, run `pbf-decoder <FILENAME`.

