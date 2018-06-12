//
// Created by Hybin on 2018/6/9.
//

#include <iostream>
#include "include/Corpus.h"

using namespace std;

string project;

int create(const string &corpusName)
{
    if (check(corpusName)) {
        cerr << "The Corpus has been created. Do not build it again!" << endl;
        return -1;
    }

    // Create a Corpus Object
    Corpus Shuuseki;
    Shuuseki.CorpusName = corpusName;
    Shuuseki.FileList = {};

    // Store the basic info of the corpus
    fstream init("init.index", ios_base::out | ios_base::in | ios_base::app);
    init << Shuuseki.CorpusName + "\n";

    // Store the data of the Corpus
    fstream corpus;
    corpus.open(corpusName + ".corpus", ios_base::out | ios_base::in | ios_base::app);
    if (corpus) {
        fstream corpus_config(corpusName + ".config", ios_base::out | ios_base::in);
        corpus_config << "Corpus Name = " << Shuuseki.CorpusName << " ;";
        corpus_config << "Corpus FileList = " << "" << " ;";
    } else {
        cerr << "-Shuuseki: Failed to create a corpus" << endl;
        return EXIT_FAILURE;
    }

    // Show status
    cout << "-Shuuseki: A corpus called [" + corpusName + "] has been created successful" << endl;

    project = Shuuseki.CorpusName;

    return 0;
}

int open(const string &corpusName)
{
    string cmd;

    // Open the corpus with read / write mode
    fstream corpus;
    corpus.open(corpusName + ".corpus", ios_base::out | ios_base::in | ios_base::app);

    if (!corpus) {
        cerr << "-Shuuseki: Failed to open a corpus" << endl;
        cout << "-Shuuseki: No such Corpus called [" + corpusName + "], wanna create it?[y/n] ";
        cin >> cmd;
        if (cmd == "y") {
            create(corpusName);
        }
    }

    project = corpusName;

    return 0;
}

int import(const vector<string> &files)
{
    fstream corpus(project + ".corpus", ios_base::out | ios_base::in | ios_base::app);
    for (auto &file : files) {
        ifstream in(file, ios_base::in);

        if (!in) {
            cerr << "File [" + file + "] not exist" << endl;
            continue;
        }

        if ((checkEncoding(file) != "UTF-8") || (checkEncoding(file) != "UTF-8 with BOM")) {
            transform(file);

            ifstream transformed("convert-output.txt", ios_base::in);
            string s;
            while (transformed >> s) {
                corpus << s;
            }
            transformed.close();
            remove("convert-output.txt");
        }

        string t;
        while (in >> t) {
            corpus << t;
        }

        in.close();
    }
    return 0;
}
