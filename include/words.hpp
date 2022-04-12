#ifndef WORDS_HPP
#define WORDS_HPP

#include <memory>
#include <unordered_map>
#include <vector>

namespace wordpath {

  /**
   * A list of words
   */
  using word_list = std::vector<std::string>;

  /**
   * Graph representation of linked words
   */
  using dict = std::unordered_map<std::string, word_list>;

  /**
   * Compute neighbours of a given word
   */
  void compute_neighbours(const std::string& source, const word_list& possible_words, std::ofstream& output, int word_count);

  /**
   * Generate dictionnary file based on raw file
   */
  void generate_dict(std::ifstream& file, std::ofstream& output);

  /**
   * Find path between two words given a dictionnary
   */
  word_list find_path(const std::string& source,
                                       const std::string& target,
                                       const dict& words);
  void print_final_list(const word_list& words);

  /**
   * Parse dictionnary from file
   */
  dict parse_dict_from_file(std::ifstream& dict_file);

}

#endif
