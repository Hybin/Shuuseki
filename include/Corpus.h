//
// Created by Hybin on 2018/6/8.
//

#ifndef SHUUSEKI_CORPUS_H
#define SHUUSEKI_CORPUS_H

#include <string>
#include <vector>
#include <fstream>
#include <cstdio>

class Corpus {
public:
    std::string CorpusName;
    std::vector<std::string> FileList;
};

//init.cpp
int initialize();
bool check(const std::string &s);

// operations.cpp
int create(const std::string &corpusName);
int open(const std::string &corpusName);
int import(const std::vector<std::string> &files);

// encoding.cpp
std::string is_unicode_or_utf8(std::string &file);
std::string is_gb_or_big5(const char * file);
std::string checkEncoding(std::string &file);

#endif //SHUUSEKI_CORPUS_H
