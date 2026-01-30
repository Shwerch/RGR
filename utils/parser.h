#pragma once

#include <filesystem>
#include <optional>
#include <vector>

enum class Algorithm {
    des,
    aes,
    ngea
};

enum class Mode {
    encrypt,
    decrypt
};

enum class Input {
    text,
    hex,
    binary
};

enum class Output {
    text,
    hex,
    binary
};

enum class ReadFormat {
    hex,
    binary
};

enum class WriteFormat {
    hex,
    binary
};

struct RawArguments {
    std::optional<Algorithm> algorithm = Algorithm::des;
    std::optional<Mode> mode = Mode::encrypt;
    std::optional<std::filesystem::path> input;
    std::optional<std::filesystem::path> output;
    std::optional<Input> input_format = Input::text;
    std::optional<Output> output_format = Output::binary;
    std::optional<std::filesystem::path> key;
    std::optional<bool> generate_key = false;
    std::optional<bool> read_key = false;
    std::optional<bool> promt_key = false;
    std::optional<ReadFormat> read_key_format = ReadFormat::binary;
    std::optional<std::filesystem::path> save_key;
    std::optional<bool> write_key = false;
    std::optional<WriteFormat> write_key_format = WriteFormat::binary;
    std::optional<bool> help = false;
};

struct Arguments {
    Algorithm algorithm = Algorithm::des;
    Mode mode = Mode::encrypt;
    std::optional<std::filesystem::path> input;
    std::optional<std::filesystem::path> output;
    Input input_format = Input::text;
    Output output_format = Output::binary;
    std::optional<std::filesystem::path> key;
    bool generate_key = false;
    bool read_key = false;
    bool promt_key = false;
    ReadFormat read_key_format = ReadFormat::binary;
    std::optional<std::filesystem::path> save_key;
    bool write_key = false;
    WriteFormat write_key_format = WriteFormat::binary;
    bool help = false;
};

constexpr auto HELP_TEXT = R"(
usage: cryptum --algorithm <alg> --mode <mode> [OPTIONS]

Uses encryption algorithms AES, DES and NGEA for data encryption and decryption.

Required:
  -a, --algorithm {des,aes,ngea}              The encryption algorithm used.
  -m, --mode {encrypt,decrypt}                Operating mode.

Input/Output Options:
  -i, --input <PATH>                          Input file path. If not provided, reads from stdin.
  -o, --output <PATH>                         Output file path. If not provided, writes to stdout.
  -I, --in-format {text,hex,binary}           Specify the format of the input data. [default: text]
  -O, --out-format {text,hex,binary}          Specify the format for stdout. Ignored if -o is used. [default: binary for encrypt, text for decrypt]

Key Input:
  -k, --key <PATH>                            Path to the key file.
  -g, --generate-key                          Generate a new random key.
  -r, --read-key                              Read key from stdin.
  -p, --promt-key                             Prompt for the key interactively on the console.
  -R, --read-key-format {hex,binary}     Specify the input format of the key. [default: binary]

Key Output:
  -S, --save-key <PATH>                       Save the generated key to a file.
  -w, --write-key                             Write the generated key to stdout.
  -W, --write-key-format {hex,binary}    Specify the output format of the key. [default: binary]

Optional:
  -h, --help                                  Show this help message and exit.
)";

RawArguments parse_arguments(int argc, const char** argv);
Arguments check_arguments(const RawArguments& args);
std::vector<uint8_t> get_key(const Arguments& args);
void export_key(const Arguments& args, const std::vector<uint8_t>& key_data);
std::vector<uint8_t> get_input(const Arguments& args);