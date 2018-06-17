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
        cerr << "-Shuuseki: The Corpus has been created. Do not build it again!" << endl;
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

int open(const string &corpusName)
{
    string cmd;

    // Open the corpus with read / write mode
    ifstream corpus;
    corpus.open(corpusName + ".corpus", ios_base::in);

    if (!corpus) {
        cerr << "-Shuuseki: Failed to open a corpus" << endl;
    } else {
        project = corpusName;
        cout << "-Shuuseki: Corpus [" + project +"] has been opened." << endl;
    }

    corpus.close();

    return 0;
}

int import(const vector<string> &files)
{
    if (project.empty()) {
        cerr << "-Shuuseki: Caution: You may need to create a Corpus or open one" << endl;
        return -1;
    }

    fstream imported_corpus(project + ".corpus", ios_base::out | ios_base::in | ios_base::app | ios_base::ate);
    if (!imported_corpus) {
        cerr << "-Shuuseki: corpus not exist" << endl;
        return -1;
    }

    ifstream imported_corpus_config(project + ".config", ios_base::in);
    if (!imported_corpus_config) {
        cerr << "-Shuuseki: corpus_config not exist" << endl;
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

    int count = 0;

    for (auto &file : files) {
        // Test if it has existed
        if (match(Shuuseki.FileList, file) != -1) {
            cerr << "-Shuuseki: The file [" + file + "] has existed." << endl;
            continue;
        }
        long long start_mark, end_mark;
        // Get the start point of the file
        imported_corpus.seekg(0, fstream::end);
        start_mark = imported_corpus.tellg();

        fstream in(file);

        if (!in) {
            cerr << "File [" + file + "] not exist" << endl;
            continue;
        }

        string encoding = checkEncoding(file);

        if ((encoding != "UTF-8") && (encoding != "UTF-8 with BOM")) {
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
            vector<string> texts;
            while (getline(in, t)) {
                texts.push_back(t);

            }

            for (auto &text : texts) {
                imported_corpus << text;
            }
        }

        end_mark = imported_corpus.tellg(); // Get the end point of the file after importing

        Shuuseki.FileList.push_back(file);
        // Store the index of the content.
        ofstream content_index(Shuuseki.CorpusName + "_content.index", ios_base::out | ios_base::app);
        content_index << "File: " << file << " ;";
        content_index << "Start Point: " << start_mark << " ;";
        content_index << "End Point: " << end_mark <<" ;";
        content_index.close();

        in.close();

        count += 1;
    }

    fstream updated_corpus_config(project + ".config", ios_base::out | ios_base::trunc);
    updated_corpus_config << "Corpus Name = " << Shuuseki.CorpusName << " ;";
    updated_corpus_config << "Corpus FileList = " << vector2string(Shuuseki.FileList) << " ;";

    cout << "-Shuuseki: Your data has been imported " + to_string(count) + " files " + "into the Corpus [" + Shuuseki.CorpusName + "] successfully" << endl;

    return 0;
}

int remove(const vector<string> &files) {
    if (project.empty()) {
        cerr << "-Shuuseki: Caution: You may need to create a Corpus or open one" << endl;
        return -1;
    }

    ifstream corpus(project + ".corpus");
    if (!corpus) {
        cerr << "-Shuuseki: corpus not exist" << endl;
        return -1;
    }

    // Initialize
    corpus.seekg(0, ifstream::beg);

    ifstream corpus_config(project + ".config");
    if (!corpus_config) {
        cerr << "-Shuuseki: corpus_config not exist. It seems that your Corpus has been broken, please create new one"
             << endl;
        return -1;
    }

    // Create an Object to store the information.
    Corpus Shuuseki;
    vector<string> basic_info = readCorpusInfo(corpus_config);

    Shuuseki.CorpusName = basic_info[3];
    if (basic_info[7] != "0")
        Shuuseki.FileList = split(basic_info[7], ",");
    else
        Shuuseki.FileList = {};

    corpus_config.close();

    ofstream temp("temp.corpus", ios_base::out | ios_base::app);

    ifstream info(Shuuseki.CorpusName + "_content.index", ios_base::in);
    vector<string> srcVec = readCorpusInfo(info);
    vector<string> indice = getAbsIndice(srcVec);     // Get all files which has been imported
    vector<vector<string>> data = getConIndice(srcVec), updated_data;
    info.close();
    int count = 0;

    for (auto &file : files) {
        // Get the file index first
        vector<string> goal;
        int pos = match(indice, file);
        if (pos == -1) continue;

        goal = data[pos];                                  // Get the index of the file

        // Get the start point of the file we want to delete
        long long start = corpus.tellg(), end = stoll(goal[1], nullptr, 10), end_mark = stoll(goal[2], nullptr, 10);
        std::string s;
        // Get the content before the file[waiting for being deleted]
        if (corpus.is_open()) {
            corpus.seekg(start);
            s.resize(end - start);
            corpus.read(&s[0], end - start);
        }

        temp << s;

        // Skip to the end point of the file[waiting for being deleted]
        corpus.seekg(end_mark);

        // Now delete the file in indice and data
        indice.erase(indice.begin() + pos);
        data.erase(data.begin() + pos);
        count += 1;
    }

    // Next update new starting points and ending points
    for (int i = 0; i < data.size(); ++i) {
        vector<string> updated_atom = data[i];
        if (i == 0) {
            updated_atom[2] = to_string(stoll(data[i][2]) - stoll(data[i][1]));
            updated_atom[1] = "0";
        } else {
            updated_atom[2] = to_string(stoll(data[i][2]) - stoll(data[i][1]) + stoll(updated_data[i - 1][2]));
            updated_atom[1] = updated_data[i - 1][2];
        }

        updated_data.push_back(updated_atom);
    }

    // Import the content surplus into temp.corpus
    vector<string> surplus;
    string residue;
    long long rest_mark;
    if (data.empty()) {
        rest_mark = 0;
    } else {
        rest_mark = stoll((data.back()).back());
    }

    while (corpus && corpus.tellg() < rest_mark && getline(corpus, residue))  {
        surplus.push_back(residue);
    }


    for (auto &r : surplus) {
        temp << r;
    }
    corpus.close();
    temp.close();

    // Store the updated content to Corpus
    ofstream updated_corpus(Shuuseki.CorpusName + ".corpus");
    ifstream compelete_temp("temp.corpus", ios_base::in);
    vector<string> texts;
    string t;

    while (!compelete_temp.eof()) {
        getline(compelete_temp, t);
        texts.push_back(t);
    }

    for (auto &text : texts) {
        updated_corpus << text;
    }

    updated_corpus.close();
    compelete_temp.close();

    remove("temp.corpus");

    // Update the index and config
    Shuuseki.FileList = indice;

    fstream updated_corpus_config(project + ".config", ios_base::out | ios_base::trunc);
    updated_corpus_config << "Corpus Name = " << Shuuseki.CorpusName << " ;";
    if (!Shuuseki.FileList.empty())
        updated_corpus_config << "Corpus FileList = " << vector2string(Shuuseki.FileList) << " ;";
    else
        updated_corpus_config << "Corpus FileList = " << "0" << " ;";

    fstream updated_index(Shuuseki.CorpusName + "_content.index", ios_base::out | ios_base::trunc);
    for (auto &piece : updated_data) {
        updated_index << "File: " << piece[0] << " ;";
        updated_index << "Start Point: " << piece[1] << " ;";
        updated_index << "End Point: " << piece[2] <<" ;";
    }

    cout << "-Shuuseki: You has delete " + to_string(count) + " files successfully from Corpus [" + Shuuseki.CorpusName + "]" << endl;

    return 0;
}
