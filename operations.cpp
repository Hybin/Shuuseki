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
    fstream init("corpora.index", ios_base::out | ios_base::in | ios_base::app);
    init << Shuuseki.CorpusName + "\n";

    // Store the data of the Corpus
    fstream corpus;
    corpus.open(corpusName + ".corpus", ios_base::out | ios_base::in | ios_base::app);

    ofstream corpus_config;
    corpus_config.open(corpusName + ".config", ios_base::out | ios_base::app);
    corpus_config << "Corpus Name = " << Shuuseki.CorpusName << " ;";
    corpus_config << "Corpus FileList = " << "0" << " ; ";

    corpus_config.close();

    if (!corpus) {
        cerr << "-Shuuseki: Failed to create a corpus" << endl;
        return -1;
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
    ifstream corpus;
    corpus.open(corpusName + ".corpus");

    if (!corpus) {
        cerr << "-Shuuseki: Failed to open a corpus" << endl;
        cout << "-Shuuseki: No such Corpus called [" + corpusName + "], wanna create it?[y/n] ";
        cin >> cmd;
        if (cmd == "y") {
            create(corpusName);
        } else if (cmd == "n") {
            return -1;
        }

    }

    corpus.close();

    project = corpusName;

    return 0;
}

int import(const vector<string> &files)
{
    if (project.empty()) {
        cerr << "You may need to create a Corpus or open one" << endl;
        return -1;
    }

    fstream corpus(project + ".corpus", ios_base::out | ios_base::in | ios_base::app);
    if (!corpus) {
        cerr << "corpus not exist" << endl;
        return -1;
    }
    ifstream corpus_config(project + ".config", ios_base::in);
    if (!corpus_config) {
        cerr << "corpus_config not exist" << endl;
        return -1;
    }

    // Create an Object to store the information.
    Corpus Shuuseki;
    vector<string> basic_info = readCorpusInfo(corpus_config);

    Shuuseki.CorpusName = basic_info[3];
    if (basic_info[7] != "0" )
        Shuuseki.FileList = split(basic_info[7], ",");
    else
        Shuuseki.FileList = {};

    corpus_config.close();

    for (auto &file : files) {
        ifstream in(file, ios_base::in);

        if (!in) {
            cerr << "File [" + file + "] not exist" << endl;
            continue;
        }

        if ((checkEncoding(file) != "UTF-8") && (checkEncoding(file) != "UTF-8 with BOM")) {
            transform(file);

            ifstream transformed("convert-output.txt", ios_base::in);
            string s;
            while (getline(transformed, s)) {
                corpus << s;
            }
            transformed.close();
            remove("convert-output.txt");
        } else {
            string t;
            while (getline(in, t)) {
                corpus << t;
            }
        }

        Shuuseki.FileList.push_back(file);

        in.close();
    }

    fstream updated_corpus_config(project + ".config", ios_base::out | ios_base::trunc);
    updated_corpus_config << "Corpus Name = " << Shuuseki.CorpusName << " ;";
    updated_corpus_config << "Corpus FileList = " << vector2string(Shuuseki.FileList) << " ;";

    cout << "Your data has been imported into the Corpus [" + Shuuseki.CorpusName + "] successfully" << endl;

    return 0;
}
