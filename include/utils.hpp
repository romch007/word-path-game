#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <words.hpp>

namespace wordpath {

  /**
   * Check if a given word is in a given word list
   */
  bool is_in(const word_list& words, const std::string& target);

  /**
   * Get a string representation of a rounded float
   */
  std::string round_to_str(float number, unsigned int precision);

  /**
   * Lowercase whole string
   */
  void lowercase(std::string& str);

  /**
   * Check if given words are all in a given dict
   */
  bool check_for_words(const word_list& words, const dict& target_word);

}

#endif
