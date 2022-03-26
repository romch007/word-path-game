#include <cxxopts.hpp>
#include <fstream>
#include <iostream>
#include <words.hpp>

int main(int argc, char** argv) {
  cxxopts::Options options("word-path-game", "One line description");
  options.add_options()("d,dict", "Dict file to use",
                        cxxopts::value<std::string>())(
      "r,raw", "Raw file to use", cxxopts::value<std::string>())(
      "s,source", "Source word", cxxopts::value<std::string>())(
      "o,output", "Output dict", cxxopts::value<std::string>())(
      "t,target", "Target word", cxxopts::value<std::string>());

  auto result = options.parse(argc, argv);

  if (result.count("raw")) {
    auto raw_filename = result["raw"].as<std::string>();
    auto output_filename = result["output"].as<std::string>();
    std::ifstream raw_file(raw_filename);

    if (!raw_file.is_open()) {
      std::cout << "Raw file not found" << std::endl;
      return 1;
    }

    std::ofstream output_file(output_filename);

    auto result_dict = generate_dict(raw_file, output_file);
    std::cout << "Dict generated" << std::endl;
  } else {
    auto dict_filename = result["dict"].as<std::string>();
    auto source_str = result["source"].as<std::string>();
    auto target_str = result["target"].as<std::string>();
    std::ifstream dict_file(dict_filename);

    if (!dict_file.is_open()) {
      std::cout << "Dict file not found" << std::endl;
      return 1;
    }

    auto imported_dict = parse_dict_from_file(dict_file);
    auto result_path =
        find_path(source_str, target_str, std::move(imported_dict));
    std::cout << "Path computed" << std::endl;

    print_final_list(std::move(result_path));
  }
  return 0;
}
