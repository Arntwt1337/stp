#include <cctype>
#include <fstream>
#include <nlohmann/json.hpp>
#include <parser/parser.hpp>

using json = nlohmann::json;
using namespace std;

struct Config get_config_from_json(const string& filename) {
  json json_conf;
  ifstream json_stream(filename);
  json_stream >> json_conf;

  struct Config config;
  for (size_t i = 0; i < json_conf["stop_words"].size(); i++) {
    config.stop_words.push_back(json_conf["stop_words"][i]);
  }

  config.ngram_min_length = json_conf["ngram_min_length"];
  config.ngram_max_length = json_conf["ngram_max_length"];

  return config;
}

static string deletePunct(string text) {
  for (size_t i = 0; i < text.size(); ++i) {
    if (ispunct(text[i])) {
      text.erase(i, 1);
    }
  }
  return text;
}

static string stringToLower(string text) {
  for (size_t i = 0; i < text.size(); ++i) {
    text[i] = (char)tolower(text[i]);
  }
  return text;
}

vector<string> parser::splitWords(string text, vector<string> words) {
  size_t pos = 0;
  for (size_t i = 0; i < text.size(); ++i) {
    if (isspace(text[i])) {
      if (i == pos) {
        pos++;
      } else {
        words.push_back(text.substr(pos, i - pos));
        pos = i + 1;
      }
    }
  }
  if (isspace(text[pos]) == 0 && pos < text.size()) {
    words.push_back(text.substr(pos));
  }
  return words;
}

static vector<string> deleteStopWords(
    vector<string> words,
    struct Config config) {
  auto iter = words.cbegin();
  for (size_t i = 0; i < words.size(); ++i) {
    for (size_t j = 0; j < config.stop_words.size(); ++j) {
      if (words[i] == config.stop_words[j]) {
        words.erase(iter + i);
      }
    }
  }
  return words;
}

static vector<pair<string, vector<int>>> generateNgrams(
    vector<string> words,
    struct Config config) {
  vector<pair<string, vector<int>>> ngrams;
  vector<pair<string, vector<int>>>::iterator iter;
  int count_pos = 0;
  bool check_ngrams = false, check_iter = false;
  for (size_t i = 0; i < words.size(); ++i) {
    for (int j = config.ngram_min_length;
         j <= config.ngram_max_length && j <= (int)words[i].size();
         ++j) {
      iter = ngrams.begin();
      for (size_t k = 0; k < ngrams.size(); k++, iter++) {
        if (ngrams[k].first == words[i].substr(0, j)) {
          check_iter = true;
          break;
        }
      }
      if (!check_iter) {
        iter = ngrams.end();
      }
      if (iter == ngrams.end()) {
        vector<int> new_vector = {count_pos};
        ngrams.push_back(make_pair(words[i].substr(0, j), new_vector));
        check_ngrams = true;
      } else {
        iter->second.push_back(count_pos);
        check_ngrams = true;
      }
    }
    if (check_ngrams) {
      ++count_pos;
      check_ngrams = false;
    }
  }
  return ngrams;
}

static string makeParsingStr(vector<pair<string, vector<int>>> ngrams) {
  string result;
  for (size_t i = 0; i < ngrams.size(); ++i) {
    result += ngrams[i].first + " ";
    for (size_t j = 0; j < ngrams[i].second.size(); ++j) {
      result += to_string(ngrams[i].second[j]) + " ";
    }
  }
  if (!result.empty()) {
    result.erase(result.size() - 1);
  }
  return result;
}

void parser::parseStr(string text) {
  text = deletePunct(text);
  text = stringToLower(text);
  vector<string> words;
  words = splitWords(text, words);
  struct Config config =
      get_config_from_json("../../../../src/parser/config.json");
  words = deleteStopWords(words, config);
  ngrams = generateNgrams(words, config);
  parsing_str = makeParsingStr(ngrams);
}

string parser::getParsingStr() {
  return parsing_str;
}

vector<pair<string, vector<int>>> parser::getNgrams() {
  return ngrams;
}