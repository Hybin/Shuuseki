//
// Created by Hybin on 2018/6/8.
//

#ifndef SHUUSEKI_CORPUS_H
#define SHUUSEKI_CORPUS_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <algorithm>
#include <utility>
#include <stdexcept>
#include <map>
#include <set>
#include <functional>
#include <locale>

class Corpus {
public:
    std::string CorpusName;
    std::vector<std::string> FileList;
};

//init.cpp
int initialize();
bool check(const std::string &s);
std::vector<std::string> split(const std::string  &s, std::string token);
int match(std::vector<std::string> &src, const std::string &s);
std::string &rtrim(std::string &s);
std::string &trim(std::string &s);
std::string &ltrim(std::string &s);
std::string string_to_hex(const std::string &input);
std::string hex_to_string(const std::string &input);
int countOccurrence(std::map<std::string, std::map<std::string, std::vector<std::vector<int>>>> inventory, std::map<std::string, int> &wordOccurrences);
std::map<std::string, std::map<std::string, std::vector<std::vector<int>>>> preprocessing(std::fstream &in, const std::string &file);
int writeIndiceIntoCorpus(std::map<std::string, std::map<std::string, std::vector<std::vector<int>>>> &inventory, std::fstream &out);
std::map<std::string, std::map<std::string, std::vector<std::vector<int>>>> getDataFromCorpus(std::fstream &in);
std::string transInput(const std::string &file);
std::string transOutput(const std::string &output);

// operations.cpp
// Create a new Corpus
int create(const std::string &corpusName);
// Open a Corpus
int open(const std::string &corpusName);
// Import file(s) into corpus
int import(const std::vector<std::string> &files);
// Remove content from corpus
int remove(const std::vector<std::string> &files);
// Show corpus status
int show(const std::string &corpusName);
// Count frequency corpus
int count(std::vector<std::string> options);
// Search in corpus
int search(std::vector<std::string> options);

// encoding.cpp
// Check the encoding of the contents
std::string is_utf8_or_utf8bom(const std::string &file);
std::string is_unicode(const std::string &file);
std::string is_gb_or_big5(const char * file);
std::string checkEncoding(const std::string &file);
int GBKorBig5toUTF8(const std::string &sentences, int codePage);

// Transform various of encoding format into UTF-8
char * unicode_to_utf8(unsigned short unicode);
int gbk_or_big5_2_utf8(char* in, const char* encoding);
int isutf8(const char *s, size_t ns);

int transform(const std::string &file);

// Sort.cpp
std::vector<std::string> getStringFromCorpus(std::fstream &in);
std::vector<std::string> removeEmptyLines(std::vector<std::string> &lines);
std::vector<std::string> splitSentence(const std::string &s);
std::map<std::string, int> n_gram(int &n_min, int &n_max, std::vector<std::string> content, int &f_min, int &f_max);
std::map<std::string, int> generate();

#endif //SHUUSEKI_CORPUS_H
