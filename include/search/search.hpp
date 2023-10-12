#pragma once
#include <iostream>
#include <string>

struct search_info {
  int df;
  std::string text;
  std::vector<std::pair<std::string, int>> tf;
};

class SearchIndex {
 private:
  std::vector<search_info> quers;
  std::vector<std::pair<std::string, double>> search_result;

 public:
  void search(std::string str, std::string path_index);
  void score(std::string doc_id);
  std::vector<std::pair<std::string, double>> getSearchResult();
};

int countDoc();
