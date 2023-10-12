#include <dirent.h>
#include <picosha2.h>
#include <cmath>
#include <fstream>
#include <index/index.hpp>
#include <parser/parser.hpp>
#include <search/search.hpp>

using namespace std;

void SearchIndex::search(string str, string path_index) {
  parser parsing_string;
  parsing_string.parseStr(str);
  string hash_hex_str, buff;
  vector<pair<string, vector<int>>> ngrams = parsing_string.getNgrams();
  ifstream index;
  vector<string> entries;
  for (size_t i = 0; i < ngrams.size(); i++) {
    picosha2::hash256_hex_string(ngrams[i].first.substr(0, 3), hash_hex_str);
    hash_hex_str = hash_hex_str.substr(0, 6);
    index.open(path_index + "/entries/" + hash_hex_str);
    if (index.is_open()) {
      while (getline(index, buff)) {
        entries = parser::splitWords(buff, entries);
        struct search_info query;
        if (ngrams[i].first == entries[0]) {
          IndexAccessor access;
          access.genDocList(entries[0]);
          query.df = stoi(entries[1]);
          query.text = entries[0];
          query.tf = access.getTermDocList();
          quers.push_back(query);
        }
        buff.clear();
        entries.clear();
      }
      index.close();
    }
  }
}

static int termFrequency(vector<pair<string, int>> query_tf, string doc_id) {
  for (size_t i = 0; i < query_tf.size(); i++) {
    if (query_tf[i].first == doc_id) {
      return query_tf[i].second;
    }
  }
  return 0;
}

int countDoc() {
  int file_count = 0;
  DIR* dirp;
  struct dirent* entry;
  dirp = opendir("index/docs");
  while ((entry = readdir(dirp)) != NULL) {
    if (entry->d_type == DT_REG) {
      file_count++;
    }
  }
  closedir(dirp);
  return file_count;
}

void SearchIndex::score(string doc_id) {
  int N = countDoc();
  double score = 0.0, idf = 0.0;
  for (size_t i = 0; i < quers.size(); i++) {
    idf = (double)N / (double)quers[i].df;
    score += termFrequency(quers[i].tf, doc_id) * log(idf);
  }
  if (score != 0) {
    search_result.push_back(make_pair(doc_id, score));
  }
}

vector<pair<string, double>> SearchIndex::getSearchResult() {
  sort(
      search_result.begin(),
      search_result.end(),
      [](const auto& x, const auto& y) { return x.second > y.second; });
  return search_result;
}
