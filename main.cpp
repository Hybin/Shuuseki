#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "include/Corpus.h"

using namespace std;

Corpus Shuuseki;
vector<Corpus> Corpora;

int create(const string &s)
{
    string corpusCmd = s.substr(0, 6), corpusName = s.substr(7, s.size() - 7);
    if (corpusCmd == "create") {
        // Create a Corpus Object
        Shuuseki.CorpusName = corpusName;

        // Store the data of the Corpus
        fstream corpus;
        corpus.open(corpusName + ".cps", ios_base::out | ios_base::app | ios_base::app);
        if (!corpus) {
            cerr << "-Shuuseki: Failed to create a corpus" << endl;
            return EXIT_FAILURE;
        }
        // Show status
        cout << "-Shuuseki: A corpus called [" + corpusName + "] has been created successful" << endl;
    } else {
        cout <<  "-Shuuseki: " + corpusCmd + ": command not found" << endl;
    }
    return 0;
}

int open(const string &s)
{
    string cmd, corpusCmd = s.substr(0, 4), corpusName = s.substr(5, s.size() - 5);

    if (corpusCmd == "open") {
        // Open the corpus with read / write mode
        fstream corpus;
        corpus.open(corpusName + ".cps", ios_base::out | ios_base::app | ios_base::app);

        if (!corpus) {
            cerr << "-Shuuseki: Failed to open a corpus" << endl;
            cout << "-Shuuseki: No such Corpus called [" + corpusName + "], wanna create it?[y/n] ";
            cin >> cmd;
            if (cmd == "y") {
                create("create " + corpusName);
            }
        }
    } else {
        cout << "-Shuuseki: " + corpusCmd + ": command not found";
    }

    return 0;
}

int main()
{
    string s1, s2;
    cout << ">>> ";
    getline(cin, s1);
    cout << create(s1) << endl;
    cout << ">>> ";
    getline(cin, s2);
    cout << open(s2) << endl;
    return 0;
}