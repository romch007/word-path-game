#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <words.hpp>

bool is_in(const word_list& words, const std::string& target);
std::string round_to_str(float number, unsigned int precision);
void lowercase(std::string& str);

#endif
