#include <picosha2.h>
#include <fstream>
#include <index/index.hpp>
#include <parser/parser.hpp>

using namespace std;

void Index::setEntries(string key, vector<term> terms) {
  entries = make_pair(key, terms);
}

pair<string, vector<term>> Index::getEntries() {
  return entries;
}

void indexBuilder::setIndexes(Index index) {
  indexes.push_back(index);
}

vector<Index> indexBuilder::getIndexes() {
  return indexes;
}

vector<pair<string, string>> indexBuilder::getDocs() {
  return docs;
}

static bool hashUniquenessCheck(string hash, vector<Index> indexes) {
  for (size_t i = 0; i < indexes.size(); ++i) {
    if (hash == indexes[i].getEntries().first) {
      return false;
    }
  }
  return true;
}

static term createTerm(pair<string, vector<int>> ngram, string doc_id) {
  term term_obj;
  term_obj.text = ngram.first;
  term_obj.doc_count = 1;
  term_obj.doc_id_and_pos.push_back(make_pair(doc_id, ngram.second));
  return term_obj;
}

void Index::correct_index(
    string doc_id,
    string hash,
    string term,
    vector<int> pos) {
  if (hash == entries.first) {
    for (size_t i = 0; i < entries.second.size(); i++) {
      if (entries.second[i].text == term) {
        entries.second[i].doc_id_and_pos.push_back(make_pair(doc_id, pos));
        entries.second[i].doc_count++;
      }
    }
  }
}

void indexBuilder::add_document(string document_id, string text) {
  parser parsing_string;
  parsing_string.parseStr(text);
  string hash_hex_str, prev_ngram;
  vector<term> entries_terms;
  vector<pair<string, vector<int>>> ngrams = parsing_string.getNgrams();
  for (size_t i = 0; i < ngrams.size(); i++) {
    picosha2::hash256_hex_string(ngrams[i].first.substr(0, 3), hash_hex_str);
    hash_hex_str = hash_hex_str.substr(0, 6);
    if (hashUniquenessCheck(hash_hex_str, indexes)) {
      prev_ngram = ngrams[i].first.substr(0, 3);
      while (i < ngrams.size() && ngrams[i].first.substr(0, 3) == prev_ngram) {
        entries_terms.push_back(createTerm(ngrams[i], document_id));
        i++;
      }
      i--;
      indexes.push_back(index(hash_hex_str, entries_terms));
      entries_terms.clear();
    } else {
      for (size_t j = 0; j < indexes.size(); j++) {
        indexes[j].correct_index(
            document_id, hash_hex_str, ngrams[i].first, ngrams[i].second);
      }
    }
  }
  docs.push_back(make_pair(document_id, text));
}

Index indexBuilder::index(string entries_key, vector<term> entries_terms) {
  Index index;
  index.setEntries(entries_key, entries_terms);
  return index;
}

bool demo_exists(const fs::path& p, fs::file_status s = fs::file_status{}) {
  if (fs::status_known(s) ? fs::exists(s) : fs::exists(p))
    return true;
  else
    return false;
}

void textIndexWriter::documentsCreate(
    string path,
    vector<pair<string, string>> docs) {
  const fs::path path_index{path};
  if (!demo_exists(path_index)) {
    fs::create_directory(path_index);
  }
  const fs::path path_docs{path + "/docs"};
  if (!demo_exists(path_docs)) {
    fs::create_directory(path_docs);
  }
  ofstream doc;
  for (size_t i = 0; i < docs.size(); i++) {
    doc.open(path + "/docs/" + docs[i].first);
    if (doc.is_open()) {
      doc << docs[i].second;
      doc.close();
    }
  }
}

void textIndexWriter::write(string path, Index index) {
  const fs::path path_index{path};
  if (!demo_exists(path_index)) {
    fs::create_directory(path_index);
  }
  const fs::path path_entries{path + "/entries"};
  if (!demo_exists(path_entries)) {
    fs::create_directory(path_entries);
  }
  ofstream entries(path + "/entries/" + index.getEntries().first);
  vector<term> terms = index.getEntries().second;
  for (size_t i = 0; i < terms.size(); ++i) {
    entries << terms[i].text << " " << terms[i].doc_count << " ";
    for (size_t j = 0; j < terms[i].doc_id_and_pos.size(); ++j) {
      entries << terms[i].doc_id_and_pos[j].first << " "
              << terms[i].doc_id_and_pos[j].second.size() << " ";
      for (size_t k = 0; k < terms[i].doc_id_and_pos[j].second.size(); ++k) {
        entries << terms[i].doc_id_and_pos[j].second[k] << " ";
      }
    }
    entries << endl;
  }
  entries.close();
}

string textIndexWriter::testIndex(Index index) {
  string result_index;
  vector<term> terms = index.getEntries().second;
  for (size_t i = 0; i < terms.size(); ++i) {
    result_index += terms[i].text + " " + to_string(terms[i].doc_count) + " ";
    for (size_t j = 0; j < terms[i].doc_id_and_pos.size(); ++j) {
      result_index += terms[i].doc_id_and_pos[j].first + " " +
          to_string(terms[i].doc_id_and_pos[j].second.size()) + " ";
      for (size_t k = 0; k < terms[i].doc_id_and_pos[j].second.size(); ++k) {
        result_index += to_string(terms[i].doc_id_and_pos[j].second[k]) + " ";
      }
    }
    result_index += "\n";
  }
  return result_index;
}

string IndexAccessor::getDocText() {
  return doc_text;
}
vector<pair<string, int>> IndexAccessor::getTermDocList() {
  return term_doc_list;
}
void IndexAccessor::readDoc(std::string doc_id) {
  ifstream document("index/docs/" + doc_id);
  string buff;
  if (document.is_open()) {
    doc_text.clear();
    while (getline(document, buff)) {
      doc_text += buff;
    }
    document.close();
  }
}

void IndexAccessor::genDocList(std::string term) {
  string hash_hex_str, buff;
  ifstream index;
  vector<string> entries;
  picosha2::hash256_hex_string(term.substr(0, 3), hash_hex_str);
  hash_hex_str = hash_hex_str.substr(0, 6);
  index.open("index/entries/" + hash_hex_str);
  if (index.is_open()) {
    term_doc_list.clear();
    while (getline(index, buff)) {
      entries = parser::splitWords(buff, entries);
      if (term == entries[0]) {
        for (size_t i = 2; i < entries.size(); i++) {
          term_doc_list.push_back(make_pair(entries[i], stoi(entries[i + 1])));
          i++;
          i += stoi(entries[i]);
        }
      }
      buff.clear();
      entries.clear();
    }
    index.close();
  }
}

vector<string> IndexAccessor::allDocNames(string indexPath) {
  vector<string> docNames;
  string tmpName;
  fs::path path;

  for (auto const& p : fs::directory_iterator(indexPath + "/docs")) {
    path = p;
    tmpName = path.fs::path::generic_string();
    tmpName = tmpName.substr(tmpName.find_last_of("/") + 1);
    docNames.push_back(tmpName);
  }

  return docNames;
}