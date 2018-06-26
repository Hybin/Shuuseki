//
// Created by Hybin on 2018/6/9.
//

#include <iostream>
#include <vector>
#include "include/Corpus.h"

using namespace std;

string project;
map<string, map<string, vector<vector<int>>>> indexes;

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
    fstream corpus;
    corpus.open(corpusName + ".corpus", ios_base::in);

    if (!corpus)
        cerr << "-Shuuseki: Failed to open a corpus" << endl;

    cout << "-Shuuseki: Please wait for a minute..." << endl;

    indexes = getDataFromCorpus(corpus);
    project = corpusName;

    corpus.close();

    cout << "-Shuuseki: Corpus [" + corpusName +"] has been opened." << endl;

    return 0;
}

int check()                // Check if a Corpus has been opened or created
{
    if (indexes.empty()) {
        cerr << "-Shuuseki: Caution: You may need to create a Corpus or open one" << endl;
        return -1;
    }
    return 0;
}

int import(const vector<string> &files)
{
    check();

    ifstream imported_corpus_config(project + ".config", ios_base::in);
    if (!imported_corpus_config) {
        cerr << "-Shuuseki: corpus_config not exist" << endl;
        return -1;
    }

    cout << "-Shuuseki: Please wait for a minute..." << endl;

    // Create an Object to store the information.
    Corpus Shuuseki;
    Shuuseki.CorpusName = project;

    for (auto &index: indexes)
        Shuuseki.FileList.push_back(index.first);

    fstream imported_corpus(project + ".corpus", ios_base::out | ios_base::in | ios_base::app | ios_base::ate);
    if (!imported_corpus) {
        cerr << "-Shuuseki: corpus not exist" << endl;
        return -1;
    }

    // Make a record of the number of imported files
    int count = 0;

    for (auto &file : files) {
        // Test if it has existed
        if (match(Shuuseki.FileList, file) != -1) {
            cerr << "-Shuuseki: The file [" + file + "] has existed." << endl;
            continue;
        }

        fstream in(file);

        if (!in) {
            cerr << "File [" + file + "] not exist" << endl;
            continue;
        }

        string encoding = checkEncoding(file);

        map<string, map<string, vector<vector<int>>>> inventory;

        if ((encoding != "UTF-8") && (encoding != "UTF-8 with BOM")) {
            transform(file);

            fstream raw("convert-output.txt", ios_base::in);
            inventory = preprocessing(raw, file);
            raw.close();

            remove("convert-output.txt");
        } else {
            inventory = preprocessing(in, file);
        }

        in.close();

        indexes.insert(inventory.begin(), inventory.end());
        writeIndiceIntoCorpus(inventory, imported_corpus);

        Shuuseki.FileList.push_back(file);
        ++count;
    }

    cout << "-Shuuseki: Your data has been imported " + to_string(count) + " files " + "into the Corpus [" + Shuuseki.CorpusName + "] successfully" << endl;

    return 0;
}

int remove(const vector<string> &files)
{

    check();
    cout << "-Shuuseki: Please wait for a minute..." << endl;

    // Create an Object to store the information.
    Corpus Shuuseki;
    Shuuseki.CorpusName = project;

    for (auto &index: indexes)
        Shuuseki.FileList.push_back(index.first);

    // Delete the content
    int count = 0, record = static_cast<int>(indexes.size());
    for (auto &file : files) {
        int pos = match(Shuuseki.FileList, file);
        if (pos == -1) continue;

        map<string, map<string, vector<vector<int>>>>::iterator item;
        item = indexes.find(file);

        if (item != indexes.end())
            indexes.erase(file);

        Shuuseki.FileList.erase(Shuuseki.FileList.begin() + pos);
        ++count;
    }

    // Write the left content into corpus
    if (indexes.size() != record) {
        fstream updated_corpus(project + ".corpus", ios_base::out | ios_base::trunc);
        writeIndiceIntoCorpus(indexes, updated_corpus);
        updated_corpus.close();
    }

    cout << "-Shuuseki: You has delete " + to_string(count) + " files successfully from Corpus [" + Shuuseki.CorpusName + "]" << endl;

    return 0;
}

// Custom Functions for vector sorting
// Sort by int
struct IntCmp {
    bool operator()(const pair<string, int> &lhs, const pair<string, int> &rhs)
    {
        return lhs.second > rhs.second;
    }
};

// Sort by Pinyin
map<string, int> HFKaiji = generate();
struct AlpCmp {
    bool operator()(const pair<string, int> &lhs, const pair<string, int> &rhs)
    {
        string lhsFirst = hex_to_string(string_to_hex(lhs.first).substr(0, 6)),
                rhsFisrt = hex_to_string(string_to_hex(rhs.first).substr(0, 6));
        if (HFKaiji[lhsFirst] && HFKaiji[rhsFisrt])
            return HFKaiji[lhsFirst] < HFKaiji[rhsFisrt];
        else
            return false;
    }
};

int show(const string &corpusName)
{
    check();

    cout << "-Shuuseki: Please wait for a minute..." << endl;

    // Create an Object to store the information.
    Corpus Shuuseki;
    Shuuseki.CorpusName = project;

    for (auto &index: indexes)
        Shuuseki.FileList.push_back(index.first);

    int charactersNum = 0;
    map<string, map<string, vector<vector<int>>>>::iterator item;
    for (item = indexes.begin(); item != indexes.end(); ++item)
        for (auto & c : item -> second)
            charactersNum += c.second.size();

    cout << "- Shuuseki: In Corpus [" + Shuuseki.CorpusName + "], there are " + to_string(Shuuseki.FileList.size()) + " imported files, "
         << "almost " + to_string(charactersNum) + " characters(Kanji or English words)." << endl;

    cout << "  And later you will see the Kanji with top 20 occurrences. Please wait for a minute......(only if your Corpus is big)" << endl;

    // Count the occurrences of characters
    map<string, int> wordOccurrences;
    countOccurrence(indexes, wordOccurrences);

    // Sort the map

    vector<pair<string, int>> result(wordOccurrences.begin(), wordOccurrences.end());

    partial_sort(result.begin(), result.begin() + 20, result.end(), IntCmp());

    for (int i = 0; i < 20; ++i) {
        cout << "  " << result[i].first << " occurs " << result[i].second << ((result[i].second > 1) ? " times" : " time") << endl;
    }

    return 0;
}

int count(vector<string> options)
{
    if (options.size() != 5 && options[0] != "-a" && options[0] != "-f") {
        cerr << "-Shuuseki: missing options. Please input command [sort] with correct options." << endl;
        return -1;
    }

    check();

    cout << "-Shuuseki: Please wait for a minute..." << endl;

    fstream corpus(project + ".corpus");
    if (!corpus) {
        cerr << "-Shuuseki: corpus not exist" << endl;
        return -1;
    }

    vector<string> lines = getStringFromCorpus(corpus), sentences = removeEmptyLines(lines), content;

    for (auto &l : sentences) {
        vector<string> words = splitSentence(l);
        content.insert(content.end(), words.begin(), words.end());
    }

    int n_min = stoi(options[1]), n_max = stoi(options[2]), f_min = stoi(options[3]), f_max = stoi(options[4]);

    map<string, int> stringOccurrences = n_gram(n_min, n_max, content, f_min, f_max);
    vector<pair<string, int>> result(stringOccurrences.begin(), stringOccurrences.end());

    ofstream out("recurrence.txt", ios_base::out | ios_base::app | ios_base::trunc);
    if (options[0] == "-f") {
        sort(result.begin(), result.end(), IntCmp());
        out << " 频次" << "\t\t" << "字符串" << endl;
    } else {
        sort(result.begin(), result.end(), AlpCmp());
        out << " 字符串" << "\t\t" << "频次" << endl;
    }

    for (auto &r : result) {
        if (r.second >= f_min && r.second <= f_max)
            out << " " << r.second << "\t\t" << r.first << endl;
    }

    out.close();
    cout << "-Shuuseki: Finished!" << endl;

    return 0;
}
