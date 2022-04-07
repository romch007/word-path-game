#include <algorithm>
#include <mutex>
#include <thread>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <utils.hpp>
#include <words.hpp>

std::mutex mtx;

void compute_neighbours(const std::string& source, const word_list& possible_words, std::ofstream& output) {
  static std::string alpha = "abcdefghijklmnopqrstuvwxyz";
  std::stringstream ss;
  ss << source;
  for (int i = 0; i < source.size(); i++) {
    for (const auto& letter : alpha) {
      std::string tmp = source;
      if (tmp[i] == letter)
        continue;
      tmp[i] = letter;
      if (is_in(possible_words, tmp)) {
        ss << " " << tmp;
      }
    }
  }
  ss << "\n";
  mtx.lock();
  output << ss.str();
  mtx.unlock();
}

void generate_dict(std::ifstream& file, std::ofstream& output) {
  word_list possible_words;

  std::string line;
  int word_count = 0;
  while (std::getline(file, line)) {
    lowercase(line);
    possible_words.push_back(line);
    word_count++;
  }
  std::cout << word_count << " words were read" << std::endl
            << "Starting dict creation" << std::endl;

  std::vector<std::thread> threads(possible_words.size());
  for (const auto& word : possible_words) {
    threads.emplace_back(compute_neighbours, word, possible_words, std::ref(output));
  }
  for (auto& t : threads) {
    if (t.joinable()) {
      t.join();
    }
  }
}

void save_dict_to_file(const dict& dict_to_save, std::ofstream& file) {
  for (const auto& elem : dict_to_save) {
    file << elem.first;
    for (const auto& linked_word : elem.second) {
      file << " " << linked_word;
    }
    file << std::endl;
  }
}

std::unique_ptr<word_list> find_path(const std::string& source,
                                     const std::string& target,
                                     std::unique_ptr<dict> words) {
  std::queue<std::string> path;
  std::unordered_map<std::string, bool> used;
  std::unordered_map<std::string, std::string> previous;
  
  {
    word_list source_and_target = { source, target };

    if (!check_for_words(source_and_target, *words)) {
      return nullptr;
    }
  }

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

std::unique_ptr<dict> parse_dict_from_file(std::ifstream& dict_file) {
  auto result = std::make_unique<dict>();

  std::string line;
  char delimiter = ' ';
  while (std::getline(dict_file, line)) {
    std::istringstream ss(line);
    std::string word;
    std::getline(ss, word, delimiter);
    result->insert(std::make_pair(word, word_list()));

    std::string linked_word;
    while (std::getline(ss, linked_word, delimiter)) {
      result->at(word).push_back(linked_word);
    }
  }
  return result;
}
