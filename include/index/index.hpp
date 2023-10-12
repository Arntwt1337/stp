#pragma once
#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

struct term {
  int doc_count;
  std::string text;
  std::vector<std::pair<std::string, std::vector<int>>> doc_id_and_pos;
};

class Index {
 private:
  std::pair<std::string, std::vector<term>> entries;

 public:
  std::pair<std::string, std::vector<term>> getEntries();
  void setEntries(std::string key, std::vector<term> terms);
  void correct_index(
      std::string doc_id,
      std::string hash,
      std::string term,
      std::vector<int> pos);
};

class indexBuilder {
 private:
  std::vector<std::pair<std::string, std::string>> docs;
  std::vector<Index> indexes;

 public:
  void add_document(std::string document_id, std::string text);
  void setIndexes(Index index);
  std::vector<Index> getIndexes();
  std::vector<std::pair<std::string, std::string>> getDocs();
  Index index(std::string entries_key, std::vector<term> entries_terms);
};

class textIndexWriter {
 public:
  static void write(std::string path, Index index);
  static void documentsCreate(
      std::string path,
      std::vector<std::pair<std::string, std::string>> docs);
  static std::string testIndex(Index index);
};

class IndexAccessor {
 private:
  std::string doc_text;
  std::vector<std::pair<std::string, int>> term_doc_list;

 public:
  std::string getDocText();
  std::vector<std::pair<std::string, int>> getTermDocList();
  void readDoc(std::string doc_id);
  void genDocList(std::string term);
  static std::vector<std::string> allDocNames(std::string indexPath);
};

bool demo_exists(const fs::path& p, fs::file_status);