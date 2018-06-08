#include <iostream>
#include <string>
#include <vector>
#include "include/Corpus.h"

using namespace std;

string create(const string &s)
{
    string res, corpusCmd = s.substr(0, 6), corpusName = s.substr(7, s.size() - 7);
    if (corpusCmd == "create") {
        Corpus Shuuseki;
        Shuuseki.CorpusName = corpusName;
        res = "-Shuuseki: A corpus called [" + corpusName + "] has been created successful";
    } else {
        res = "-Shuuseki: " + corpusCmd + ": command not found";
    }
    return res;
}

int main()
{
    string s;
    cout << ">>> ";
    getline(cin, s);
    cout << create(s) << endl;
    return 0;
}