#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

using word_list = std::vector<std::string>;
using dict = std::map<std::string, word_list>;

std::string round_to_str(float number, unsigned int precision) {
  std::ostringstream ss;
  ss << std::fixed << std::setprecision(precision) << number;
  return ss.str();
}

bool is_in(const word_list& words, const std::string& target) {
  return std::find(words.begin(), words.end(), target) != words.end();
}

std::unique_ptr<dict> generate_dict(std::ifstream& file) {
  std::string alpha = "abcdefghijklmnopqrstuvwxyz";
  word_list possible_words;

  std::string line;
  int word_count = 0;
  while (std::getline(file, line)) {
    possible_words.push_back(line);
    word_count++;
  }
  std::cout << word_count << " words were read" << std::endl << std::endl;

  auto result = std::make_unique<dict>();
  int progress = 0;
  for (const auto& word : possible_words) {
    float progress_percentage = ((float)progress / (float)word_count) * 100.0f;
    std::cout << "Progress: " << round_to_str(progress_percentage, 2) << "%\r";
    result->insert(std::make_pair(word, word_list()));
    for (int i = 0; i < word.size(); i++) {
      for (const auto& letter : alpha) {
        std::string tmp = word;
        if (tmp[i] == letter)
          continue;
        tmp[i] = letter;
        if (is_in(possible_words, tmp))
          result->at(word).push_back(tmp);
      }
    }
    progress++;
  }
  return result;
}

std::unique_ptr<word_list> find_path(const std::string& source,
                                     const std::string& target,
                                     std::unique_ptr<dict> words) {
  std::queue<std::string> path;
  std::map<std::string, bool> used;
  std::map<std::string, std::string> previous;

  path.push(source);
  used[source] = true;
  previous[source] = "";

  while (!path.empty()) {
    auto v = path.front();
    path.pop();
    for (const auto& u : words->at(v)) {
      if (!used[u]) {
        used[u] = true;
        path.push(u);
        previous[u] = v;
      }
    }
  }

  if (!used[target]) {
    return nullptr;
  } else {
    auto reverse_path = std::make_unique<word_list>();
    for (std::string v = target; v != ""; v = previous[v])
      reverse_path->push_back(v);
    std::reverse(reverse_path->begin(), reverse_path->end());
    return reverse_path;
  }

  return nullptr;
}

void print_final_list(std::unique_ptr<word_list> words) {
  if (words == nullptr) {
    std::cout << "No path found" << std::endl;
  } else {
    std::cout << "-- FROM " << words->front() << std::endl;
    for (int i = 1; i < words->size() - 1; i++) {
      std::cout << i << ". " << words->at(i) << std::endl;
    }
    std::cout << "-- TO " << words->back() << std::endl;
  }
}

int main(int argc, char** argv) {
  if (argc < 4)
    std::cout << "Not enough arguments" << std::endl;

  std::string filename(argv[1]);
  std::ifstream file(filename);

  if (!file.is_open()) {
    std::cout << "Dict file not found" << std::endl;
    return 1;
  }

  auto result_dict = generate_dict(file);
  std::cout << "Dict generated" << std::endl;

  std::string start_word(argv[2]);
  std::string end_word(argv[3]);
  auto result_path = find_path(start_word, end_word, std::move(result_dict));
  std::cout << "Path computed" << std::endl;

  print_final_list(std::move(result_path));
  return 0;
}
