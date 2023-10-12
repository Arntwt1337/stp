#include <rapidcsv.h>
#include <CLI/CLI.hpp>
#include <index/index.hpp>
#include <iostream>
#include <search/search.hpp>

using namespace std;

int main(int argc, char** argv) {
  CLI::App fts{"Full-text search"};
  string csvPath;
  string indexPath;
  string query;

  auto indexMode = fts.add_subcommand("index", "App mode for index");
  auto searchMode = fts.add_subcommand("search", "App mode for search");
  indexMode->add_option("--csv", csvPath, "csv path")->required();
  indexMode->add_option("--index", indexPath, "index path")->required();
  searchMode->add_option("--index", indexPath, "index path")->required();
  searchMode->add_option("--query", query, "query text")->required();
  fts.require_subcommand(1, 1);
  try {
    fts.parse(argc, argv);
  } catch (CLI::Error& e) {
    return fts.exit(e);
  }

  if (fts.got_subcommand("index")) {
    rapidcsv::Document book(csvPath);
    vector<string> booksId = book.GetColumn<string>("bookID");
    vector<string> booksName = book.GetColumn<string>("title");

    indexBuilder index_builder;
    for (size_t i = 0; i < 1000; i++) {
      index_builder.add_document(booksId[i], booksName[i]);
    }

    vector<Index> indexes = index_builder.getIndexes();
    vector<pair<string, string>> documets = index_builder.getDocs();
    for (size_t i = 0; i < documets.size(); ++i) {
      textIndexWriter::documentsCreate(indexPath, documets);
    }
    for (size_t i = 0; i < indexes.size(); ++i) {
      textIndexWriter::write(indexPath, indexes[i]);
    }
  } else if (fts.got_subcommand("search")) {
    SearchIndex book_search;
    book_search.search(query, indexPath);
    vector<string> docsNames = IndexAccessor::allDocNames(indexPath);
    for (size_t i = 0; i < docsNames.size(); i++) {
      book_search.score(docsNames[i]);
    }
    cout << "id" << "\t" << "score" << "\t" << "text" << endl;
    IndexAccessor index_access;
    vector<pair<string, double>> result = book_search.getSearchResult();
    for (size_t i = 0; i < result.size(); i++) {
      index_access.readDoc(result[i].first);
      cout << result[i].first << "\t" << result[i].second << "\t"
           << index_access.getDocText() << endl;
    }
  }
  return 0;
}