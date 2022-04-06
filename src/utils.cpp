#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <utils.hpp>

void lowercase(std::string& str) {
  std::transform(str.begin(), str.end(), str.begin(),
                 [](unsigned char c) { return std::tolower(c); });
}

std::string round_to_str(float number, unsigned int precision) {
  std::ostringstream ss;
  ss << std::fixed << std::setprecision(precision) << number;
  return ss.str();
}

bool is_in(const word_list& words, const std::string& target) {
  return std::find(words.begin(), words.end(), target) != words.end();
}

bool check_for_words(const word_list& words, const dict& target_dict) {
  for (const auto& current_word : words) {
    if (target_dict.find(current_word) == target_dict.end()) {
      std::cout << "Cannot find '" << current_word << "' in dictionnary" << std::endl;
      return false;
    }
  }
  return true;
}
