#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <atomic>
#include <sstream>
#include <utils.hpp>
#include <words.hpp>
#include <threadpool/pool.hpp>

namespace wordpath {

  std::mutex mtx;
  std::atomic_int progress = 0;

  void compute_neighbours(const std::string& source, const word_list& possible_words, std::ofstream& output, int word_count) {
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

    progress++;
    float progress_percentage = ((float)progress / (float)word_count) * 100.0f;

    mtx.lock();
    output << ss.str();
    std::cout << "Progress: " << round_to_str(progress_percentage, 2) << "%\r";
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
    // std::vector<std::thread> threads(possible_words.size());
    // for (const auto& word : possible_words) {
    //   threads.emplace_back(compute_neighbours, word, possible_words, std::ref(output), word_count);
    // }
    // for (auto& t : threads) {
    //   if (t.joinable()) {
    //     t.join();
    //   }
    // }
    threadpool::pool pool;
    for (const auto& word : possible_words) {
        pool.push([&] {
            compute_neighbours(word, possible_words, std::ref(output), word_count);
        });
    }
    pool.start();
    while (pool.busy());
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

  word_list find_path(const std::string& source,
                                       const std::string& target,
                                       const dict& words) {
    std::queue<std::string> path;
    std::unordered_map<std::string, bool> used;
    std::unordered_map<std::string, std::string> previous;
    
    {
      word_list source_and_target = { source, target };

      if (!check_for_words(source_and_target, words)) {
        return word_list(0);
      }
    }

    path.push(source);
    used[source] = true;
    previous[source] = "";

    while (!path.empty()) {
      auto v = path.front();
      path.pop();
      for (const auto& u : words.at(v)) {
        if (!used[u]) {
          used[u] = true;
          path.push(u);
          previous[u] = v;
        }
      }
    }

    if (!used[target]) {
      return word_list(0);
    } else {
      word_list reverse_path;
      for (std::string v = target; v != ""; v = previous[v])
        reverse_path.push_back(v);
      std::reverse(reverse_path.begin(), reverse_path.end());
      return reverse_path;
    }
  }

  void print_final_list(const word_list& words) {
    if (words.empty()) {
      std::cout << "No path found" << std::endl;
    } else {
      std::cout << "-- FROM " << words.front() << std::endl;
      for (int i = 1; i < words.size() - 1; i++) {
        std::cout << i << ". " << words.at(i) << std::endl;
      }
      std::cout << "-- TO " << words.back() << std::endl;
    }
  }

  dict parse_dict_from_file(std::ifstream& dict_file) {
    dict result;

    std::string line;
    char delimiter = ' ';
    while (std::getline(dict_file, line)) {
      std::istringstream ss(line);
      std::string word;
      std::getline(ss, word, delimiter);
      result.insert(std::make_pair(word, word_list()));

      std::string linked_word;
      while (std::getline(ss, linked_word, delimiter)) {
        result.at(word).push_back(linked_word);
      }
    }
    return result;
  }

}
