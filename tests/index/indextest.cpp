#include <gtest/gtest.h>
#include <index/index.hpp>

using namespace std;

TEST(index, normal_data) {
  vector<string> file_name = {"199903", "200305", "200311"};
  vector<string> file_text = {
      "The Matrix", "The Matrix Reloaded", "The Matrix Revolutions"};
  indexBuilder index_builder;
  for (size_t i = 0; i < file_name.size(); i++) {
    index_builder.add_document(file_name[i], file_text[i]);
  }
  int i = 0;
  vector<string> expect = {
      "mat 3 199903 1 0 200305 1 0 200311 1 0 \nmatr 3 199903 1 0 200305 1 0 "
      "200311 1 0 \nmatri 3 199903 1 0 200305 1 0 200311 1 0 \nmatrix 3 199903 "
      "1 0 200305 1 0 200311 1 0 \n",
      "rel 1 200305 1 1 \nrelo 1 200305 1 1 \nreloa 1 200305 1 1 \nreload 1 "
      "200305 1 1 \n",
      "rev 1 200311 1 1 \nrevo 1 200311 1 1 \nrevol 1 200311 1 1 \nrevolu 1 "
      "200311 1 1 \n"};
  vector<Index> indexes = index_builder.getIndexes();
  for (size_t j = 0; j < indexes.size(); ++j) {
    ASSERT_EQ(expect[i], textIndexWriter::testIndex(indexes[j]));
    i++;
  }
}

TEST(index, repeating_term_in_one_file) {
  vector<string> file_name = {"199903", "200305", "200311"};
  vector<string> file_text = {
      "The Matrix", "The Matrix Reloaded Matr", "The Matrix Revolutions"};
  indexBuilder index_builder;
  for (size_t i = 0; i < file_name.size(); i++) {
    index_builder.add_document(file_name[i], file_text[i]);
  }
  int i = 0;
  vector<string> expect = {
      "mat 3 199903 1 0 200305 2 0 2 200311 1 0 \nmatr 3 199903 1 0 200305 2 0 "
      "2 200311 1 0 \nmatri 3 199903 1 0 200305 1 0 200311 1 0 \nmatrix 3 "
      "199903 1 0 200305 1 0 200311 1 0 \n",
      "rel 1 200305 1 1 \nrelo 1 200305 1 1 \nreloa 1 200305 1 1 \nreload 1 "
      "200305 1 1 \n",
      "rev 1 200311 1 1 \nrevo 1 200311 1 1 \nrevol 1 200311 1 1 \nrevolu 1 "
      "200311 1 1 \n"};
  vector<Index> indexes = index_builder.getIndexes();
  for (size_t j = 0; j < indexes.size(); ++j) {
    ASSERT_EQ(expect[i], textIndexWriter::testIndex(indexes[j]));
    i++;
  }
}
