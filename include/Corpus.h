//
// Created by Hybin on 2018/6/8.
//

#ifndef SHUUSEKI_CORPUS_H
#define SHUUSEKI_CORPUS_H

#include <string>
#include <vector>

using namespace std;

class Corpus {
public:
    string CorpusName;
    vector<string> FileList;
    bool CorpusModified = false;
};

//init.cpp
int initialize();
bool check(const string &s);

// operations.cpp
int create(const string &corpusName);
int open(const string &corpusName);
int import(const vector<string> &files);


#endif //SHUUSEKI_CORPUS_H
