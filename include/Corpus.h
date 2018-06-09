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

int create(const string &s);
int open(const string &s);


#endif //SHUUSEKI_CORPUS_H
