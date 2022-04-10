#ifndef WORDS_HPP
#define WORDS_HPP

#include <memory>
#include <unordered_map>
#include <vector>
#include <atomic>

namespace wordpath {

  using word_list = std::vector<std::string>;
  using dict = std::unordered_map<std::string, word_list>;

  void compute_neighbours(const std::string& source, const word_list& possible_words, std::ofstream& output, std::atomic_int& progess, int word_count);
  void generate_dict(std::ifstream& file, std::ofstream& output);
  std::unique_ptr<word_list> find_path(const std::string& source,
                                       const std::string& target,
                                       std::unique_ptr<dict> words);
  void print_final_list(std::unique_ptr<word_list> words);
  std::unique_ptr<dict> parse_dict_from_file(std::ifstream& dict_file);

}

#endif
