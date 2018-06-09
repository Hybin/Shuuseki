//
// Created by Hybin on 2018/6/9.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "include/Corpus.h"

using namespace std;

int create(const string &corpusName)
{
    if (check(corpusName)) {
        cerr << "The Corpus has been created. Do not build it again!" << endl;
        return -1;
    }

    // Create a Corpus Object
    Corpus Shuuseki;
    Shuuseki.CorpusName = corpusName;

    // Store the basic info of the corpus
    fstream init("init.index", ios_base::out | ios_base::in | ios_base::app);
    init << Shuuseki.CorpusName + "\n";

    // Store the data of the Corpus
    fstream corpus;
    corpus.open(corpusName + ".corpus", ios_base::out | ios_base::in | ios_base::app);
    if (!corpus) {
        cerr << "-Shuuseki: Failed to create a corpus" << endl;
        return EXIT_FAILURE;
    }

    // Show status
    cout << "-Shuuseki: A corpus called [" + corpusName + "] has been created successful" << endl;

    // TODO: Import data into the corpus

    return 0;
}

int open(const string &corpusName)
{
    string cmd;

    // Open the corpus with read / write mode
    fstream corpus;
    corpus.open(corpusName + ".cps", ios_base::out | ios_base::in | ios_base::app);

    if (!corpus) {
        cerr << "-Shuuseki: Failed to open a corpus" << endl;
        cout << "-Shuuseki: No such Corpus called [" + corpusName + "], wanna create it?[y/n] ";
        cin >> cmd;
        if (cmd == "y") {
            create(corpusName);
        }
    }

    return 0;
}

int import(const vector<string> &files)
{
    for (auto &file : files) {
        ifstream in(file, ios_base::in);
        if (!in) {
            cerr << "File [" + file + "] not exist" << endl;
            continue;
        }
    }
    return 0;
}
