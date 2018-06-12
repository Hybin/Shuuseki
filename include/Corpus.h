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


#endif //SHUUSEKI_CORPUS_H
