#pragma once
#include <iostream>
#include <string>
#include <vector>

struct Config {
  std::vector<std::string> stop_words;
  int ngram_min_length;
  int ngram_max_length;
};

class parser {
 private:
  std::vector<std::pair<std::string, std::vector<int>>> ngrams;
  std::string parsing_str;

 public:
  void parseStr(std::string text);
  void createNewVector(int first_pos);
  std::string getParsingStr();
  std::vector<std::pair<std::string, std::vector<int>>> getNgrams();
  static std::vector<std::string> splitWords(
      std::string text,
      std::vector<std::string> words);
};