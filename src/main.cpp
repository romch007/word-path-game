#include <cxxopts.hpp>
#include <fstream>
#include <iostream>
#include <utils.hpp>
#include <words.hpp>

int main(int argc, char** argv) {
  cxxopts::Options options("word-path-game", "One line description");
  options.add_options()
    ("d,dict", "Dict file to use", cxxopts::value<std::string>())(
      "r,raw", "Raw file to use",  cxxopts::value<std::string>())(
      "s,start", "Start word",     cxxopts::value<std::string>())(
      "o,output", "Output dict",   cxxopts::value<std::string>())(
      "e,end", "End word",         cxxopts::value<std::string>())(
      "h,help", "Display help");

  auto result = options.parse(argc, argv);

  if (result.count("raw") && result.count("output")) {
    auto raw_filename = result["raw"].as<std::string>();
    auto output_filename = result["output"].as<std::string>();
    std::ifstream raw_file(raw_filename);

    if (!raw_file.is_open()) {
      std::cout << "Raw file not found" << std::endl;
      return 1;
    }

    std::ofstream output_file(output_filename);

    wordpath::generate_dict(raw_file, output_file);
    std::cout << "Dict generated" << std::endl;
  } else if (result.count("dict") && result.count("start") &&
             result.count("end")) {
    auto dict_filename = result["dict"].as<std::string>();
    auto start_str = result["start"].as<std::string>();
    wordpath::lowercase(start_str);
    auto end_str = result["end"].as<std::string>();
    wordpath::lowercase(end_str);
    std::ifstream dict_file(dict_filename);

    if (!dict_file.is_open()) {
      std::cout << "Dict file not found" << std::endl;
      return 1;
    }

    auto imported_dict = wordpath::parse_dict_from_file(dict_file);
    auto result_path = wordpath::find_path(start_str, end_str, std::move(imported_dict));
    std::cout << "Path computed" << std::endl;

    wordpath::print_final_list(std::move(result_path));
  } else if (result.count("help")) {
    std::cout << options.help() << std::endl;
  } else {
    std::cout << "Bad arguments" << std::endl;
    return 1;
  }
  return 0;
}
