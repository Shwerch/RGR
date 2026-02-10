#pragma once

#include <string>

enum class Algorithm { DES, AES, NGEA };

enum class Mode { Encrypt, Decrypt };

struct Arguments {
	Algorithm algorithm;
	Mode mode;
	std::string inputPath;
	std::string outputPath;
	std::string keyPath;
	bool generateKey = false;
	bool readKey = false;
	std::string saveKeyPath;
	bool writeKey = false;
	bool manualInput = false;
  std::string iv;
};

const auto HELP_MESSAGE = R"(
usage: cryptum --algorithm <algorithm> --mode <mode> [OPTIONS]

Uses encryption algorithms AES, DES and NGEA for data encryption and decryption.

Required:
  -a, --algorithm {des,aes,ngea}    The encryption algorithm used.
  -m, --mode {encrypt,decrypt}      Operating mode.

Input/Output Options:
  -i, --input <PATH>                Input file path. If not provided, reads from stdin.
  -o, --output <PATH>               Output file path. If not provided, writes to stdout.
  --manual-input                    Baby mode for the lazy tester. Blocks stdin and stdout.

Key Input:
  -k, --key <PATH>                  Path to the key file.
  -g, --generate-key                Generate a new random key.
  -r, --read-key                    Read key from stdin.

Key Output:
  -s, --save-key <PATH>             Save the generated key to a file.
  -w, --write-key                   Write the generated key to stdout.

Optional:
  -h, --help                        Show this help message and exit.

One command-line argument from the Key Input category must be specified.

)";

Arguments parser(int argc, char **argv);
