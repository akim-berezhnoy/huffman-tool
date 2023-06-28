#include "decoder.h"
#include "encoder.h"

#include <boost/program_options.hpp>

#include <cerrno>
#include <fstream>
#include <iostream>

using std::ifstream;
using std::ofstream;
using std::pair;
using std::string;

using namespace huffman;

template <typename... Args>
void out(const Args&... args) {
  std::cout << '\n';
  (std::cout << ... << args);
  std::cout << "\n\n"
            << "Use --help flag to get utility functional info." << '\n';
}

template <typename... Args>
void err(const Args&... args) {
  std::cerr << '\n';
  (std::cerr << ... << args);
  std::cerr << "\n\n"
            << "Use --help flag to get utility functional info." << '\n';
}

namespace po = boost::program_options;

pair<po::options_description, po::variables_map> configure_program_options(int argc, char** argv) {
  po::options_description desc("Allowed options");
  desc.add_options()                                              //
      ("compress", "compress input file")                         //
      ("decompress", "decompress input file")                     //
      ("input", po::value<string>(), "specify input file path")   //
      ("output", po::value<string>(), "specify output file path") //
      ("help", "show utility functional");                        //

  po::positional_options_description pos_opt_desc;
  po::variables_map cmd_options;
  try {
    po::store(po::command_line_parser(argc, argv).options(desc).positional(pos_opt_desc).run(), cmd_options);
    po::notify(cmd_options);
  } catch (const std::exception& e) {
    err(e.what());
    exit(2);
  }
  return {desc, cmd_options};
}

enum class mode {
  COMPRESSION,
  DECOMPRESSION
};

int main(int argc, char** argv) {

  auto [desc, cmd_opts] = configure_program_options(argc, argv);

  if (cmd_opts.contains("help")) {
    out(desc);
    return 1;
  }

  mode program_mode;

  if (cmd_opts.contains("compress") && cmd_opts.contains("decompress")) {
    err("Program mode (flags --compress/--decompress) was defined twice. Unexpected argument pack.");
    return 1;
  } else if (cmd_opts.contains("compress")) {
    program_mode = mode::COMPRESSION;
  } else if (cmd_opts.contains("decompress")) {
    program_mode = mode::DECOMPRESSION;
  } else {
    err("Program mode (flags --compress/--decompress) was not defined.");
    return 1;
  }

  string input_file_path;

  if (cmd_opts.contains("input")) {
    input_file_path = cmd_opts["input"].as<string>();
  } else {
    out("Input file parameter (flag --input) was not defined.");
    return 0;
  }

  string output_file_path;

  if (cmd_opts.contains("output")) {
    output_file_path = cmd_opts["output"].as<string>();
  } else {
    out("Output file parameter (flag --output) was not defined.");
    return 0;
  }

  try {
    ifstream input_file(input_file_path, std::ios::binary);

    if (input_file.fail()) {
      err("Error occurred: failed to open ", input_file_path);
      return EIO;
    }

    input_file.exceptions(std::ifstream::goodbit);

    ofstream output_file(output_file_path, std::ios::binary);

    if (output_file.fail()) {
      err("Error occurred: failed to open ", output_file_path);
      input_file.close();
      return EIO;
    }

    output_file.exceptions(std::ifstream::goodbit);

    if (program_mode == mode::COMPRESSION) {
      encode(input_file, output_file);
    } else {
      decode(input_file, output_file);
    }

    // input_file.close();
    // output_file.close();

  } catch (const std::exception& e) {
    err(e.what());
    return 2;
  } catch (...) {
    err("Unexpected error happened");
    return 2;
  }
}
