//
// Created by Hybin on 2018/6/8.
//

#ifndef SHUUSEKI_CORPUS_H
#define SHUUSEKI_CORPUS_H

#include <string>
#include <vector>
#include <fstream>
#include <cstdio>
#include <algorithm>

class Corpus {
public:
    std::string CorpusName;
    std::vector<std::string> FileList;
};

//init.cpp
int initialize();
bool check(const std::string &s);
std::vector<std::string> readCorpusInfo(std::ifstream &in);
std::vector<std::string> split(const std::string  &s, std::string token);
std::string vector2string(std::vector<std::string> vec);
std::vector<std::string> getAbsIndice(std::vector<std::string> &src);
std::vector<std::vector<std::string>> getConIndice(std::vector<std::string> &src);
int match(std::vector<std::string> &src, const std::string &s);

// operations.cpp
int create(const std::string &corpusName);
int open(const std::string &corpusName);
int import(const std::vector<std::string> &files);
int remove(const std::vector<std::string> &files);

// encoding.cpp
// Check the encoding of the contents
std::string is_utf8_or_utf8bom(const std::string &file);
std::string is_unicode(const std::string &file);
std::string is_gb_or_big5(const char * file);
std::string checkEncoding(const std::string &file);

// Transform various of encoding format into UTF-8
char * unicode_to_utf8(unsigned short unicode);
int gbk_or_big5_2_utf8(char* in, const char* encoding);
int isutf8(const char *s, size_t ns);

int transform(const std::string &file);

std::vector<std::string> getStingFromCorpus(std::string &corpus);
std::vector<std::string> removeEmptyLines(std::vector<std::string> &lines)

#endif //SHUUSEKI_CORPUS_H
