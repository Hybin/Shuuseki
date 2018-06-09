#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "include/Corpus.h"

using namespace std;

string create(const string &s)
{
    string res, corpusCmd = s.substr(0, 6), corpusName = s.substr(7, s.size() - 7);
    if (corpusCmd == "create") {
        // Create a Corpus Object
        Corpus Shuuseki;
        Shuuseki.CorpusName = corpusName;

        // Store the data of the Corpus
        ofstream out;
        out.open(corpusName + ".cps", ios_base::out | ios_base::app);
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