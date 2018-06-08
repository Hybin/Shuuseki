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

string create(const string &s);


#endif //SHUUSEKI_CORPUS_H
