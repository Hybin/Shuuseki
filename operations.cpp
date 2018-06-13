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
    }

    // Show status
    cout << "-Shuuseki: A corpus called [" + corpusName + "] has been created successful" << endl;

    project = Shuuseki.CorpusName;

    return 0;
}

int openCorpus(const string &corpusName)
{
    string cmd;

    // Open the corpus with read / write mode
    ifstream corpus;
    corpus.open(corpusName + ".corpus");

    if (!corpus) {
        cerr << "-Shuuseki: Failed to open a corpus" << endl;
    }

    return 0;
}

int import(const vector<string> &files)
{
    if (project.empty()) {
        cerr << "Caution: You may need to create a Corpus or open one" << endl;
        return -1;
    }

    fstream imported_corpus(project + ".corpus", ios_base::out | ios_base::in | ios_base::app | ios_base::ate);
    if (!imported_corpus) {
        cerr << "corpus not exist" << endl;
        return -1;
    }

    ifstream imported_corpus_config(project + ".config", ios_base::in);
    if (!imported_corpus_config) {
        cerr << "corpus_config not exist" << endl;
        return -1;
    }

    // Create an Object to store the information.
    Corpus Shuuseki;
    vector<string> basic_info = readCorpusInfo(imported_corpus_config);

    Shuuseki.CorpusName = basic_info[3];
    if (basic_info[7] != "0" )
        Shuuseki.FileList = split(basic_info[7], ",");
    else
        Shuuseki.FileList = {};

    imported_corpus_config.close();

    for (auto &file : files) {
        long long start_mark, end_mark;
        // Get the start point of the file
        imported_corpus.seekg(0, fstream::end);
        start_mark = imported_corpus.tellg();

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
                imported_corpus << s;
            }
            transformed.close();
            remove("convert-output.txt");
        } else {
            string t;
            while (getline(in, t)) {
                imported_corpus << t;
            }
        }

        end_mark = imported_corpus.tellg(); // Get the end point of the file after importing

        Shuuseki.FileList.push_back(file);

        ofstream content_index(Shuuseki.CorpusName + "_content.index", ios_base::out | ios_base::app);
        content_index << "File: " << file << " ;";
        content_index << "Start Point: " << start_mark << " ;";
        content_index << "End Point: " << end_mark <<" ;";
        content_index.close();

        in.close();
    }

    fstream updated_corpus_config(project + ".config", ios_base::out | ios_base::trunc);
    updated_corpus_config << "Corpus Name = " << Shuuseki.CorpusName << " ;";
    updated_corpus_config << "Corpus FileList = " << vector2string(Shuuseki.FileList) << " ;";

    cout << "Your data has been imported into the Corpus [" + Shuuseki.CorpusName + "] successfully" << endl;

    return 0;
}

int deleteCorpus(const vector<string> &files);

