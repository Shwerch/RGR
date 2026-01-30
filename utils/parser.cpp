#include "parser.h"
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstring>


const char* HELP_MESSAGE = R"(usage: cryptum --algorithm <algorithm> --mode <mode> [OPTIONS]

Uses encryption algorithms AES, DES and NGEA for data encryption and decryption.

Required:
  -a, --algorithm {des,aes,ngea}    The encryption algorithm used.
  -m, --mode {encrypt,decrypt}      Operating mode.

Input/Output Options:
  -i, --input <PATH>                Input file path. If not provided, reads from stdin.
  -o, --output <PATH>               Output file path. If not provided, writes to stdout.
  -I, --text-input                  Specify the text format of the input data. If not provided, used binary format.
  -O, --text-output                 Specify the text format of the output data. If not provided, used binary format.

Key Input:
  -k, --key <PATH>                  Path to the key file.
  -g, --generate-key                Generate a new random key.
  -r, --read-key                    Read key from stdin.

Key Output:
  -s, --save-key <PATH>             Save the generated key to a file.
  -w, --write-key                   Write the generated key to stdout.

Optional:
  -h, --help                        Show this help message and exit.
)";

void print_help() {
    std::cout << HELP_MESSAGE;
    std::exit(0);
}

Arguments parser(int argc, char** argv) {
    if (argc == 1) {
        print_help();
    }

    Arguments args;
    std::optional<Algorithm> algoOpt;
    std::optional<Mode> modeOpt;
    bool algoSet = false;
    bool modeSet = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            print_help();
        } else if (arg == "-a" || arg == "--algorithm") {
            if (i + 1 >= argc) throw std::runtime_error("Missing argument for --algorithm");
            std::string val = argv[++i];
            if (val == "des") algoOpt = Algorithm::DES;
            else if (val == "aes") algoOpt = Algorithm::AES;
            else if (val == "ngea") algoOpt = Algorithm::NGEA;
            else throw std::runtime_error("Invalid algorithm");
            algoSet = true;
        } else if (arg == "-m" || arg == "--mode") {
            if (i + 1 >= argc) throw std::runtime_error("Missing argument for --mode");
            std::string val = argv[++i];
            if (val == "encrypt") modeOpt = Mode::Encrypt;
            else if (val == "decrypt") modeOpt = Mode::Decrypt;
            else throw std::runtime_error("Invalid mode");
            modeSet = true;
        } else if (arg == "-i" || arg == "--input") {
            if (i + 1 >= argc) throw std::runtime_error("Missing argument for --input");
            args.inputPath = argv[++i];
        } else if (arg == "-o" || arg == "--output") {
            if (i + 1 >= argc) throw std::runtime_error("Missing argument for --output");
            args.outputPath = argv[++i];
        } else if (arg == "-I" || arg == "--text-input") {
            args.textInput = true;
        } else if (arg == "-O" || arg == "--text-output") {
            args.textOutput = true;
        } else if (arg == "-k" || arg == "--key") {
            if (i + 1 >= argc) throw std::runtime_error("Missing argument for --key");
            args.keyPath = argv[++i];
        } else if (arg == "-g" || arg == "--generate-key") {
            args.generateKey = true;
        } else if (arg == "-r" || arg == "--read-key") {
            args.readKey = true;
        } else if (arg == "-s" || arg == "--save-key") {
            if (i + 1 >= argc) throw std::runtime_error("Missing argument for --save-key");
            args.saveKeyPath = argv[++i];
        } else if (arg == "-w" || arg == "--write-key") {
            args.writeKey = true;
        } else {
            throw std::runtime_error("Unknown argument: " + arg);
        }
    }

    if (!algoSet) throw std::runtime_error("Algorithm is required");
    if (!modeSet) throw std::runtime_error("Mode is required");

    args.algorithm = algoOpt.value();
    args.mode = modeOpt.value();

    int keyMethods = 0;
    if (!args.keyPath.empty()) keyMethods++;
    if (args.generateKey) keyMethods++;
    if (args.readKey) keyMethods++;

    if (keyMethods != 1) {
        throw std::runtime_error("Exactly one key source must be specified (--key, --generate-key, --read-key)");
    }

    if (args.inputPath.empty() && args.readKey) {
        throw std::runtime_error("Cannot read both input and key from stdin");
    }

    if (args.outputPath.empty() && args.writeKey) {
        throw std::runtime_error("Cannot write both output and key to stdout");
    }

    return args;
}
