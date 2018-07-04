//
// Created by Hybin on 2018/6/9.
//

#include <iostream>
#include <vector>
#include <sstream>
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

    init.close();

    // Create a file for the Corpus
    fstream corpus;
    corpus.open(corpusName + ".corpus", ios_base::out | ios_base::in | ios_base::app);

    if (!corpus) {
        cerr << "-Shuuseki: Failed to create a corpus" << endl;
    }
    corpus.close();
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

    if (!corpus) {
        cerr << "-Shuuseki: Failed to open a corpus" << endl;
        return -1;
    }

    cout << "-Shuuseki: Please wait for a minute..." << endl;

    indexes = getDataFromCorpus(corpus);
    project = corpusName;

    corpus.close();

    cout << "-Shuuseki: Corpus [" + corpusName +"] has been opened." << endl;

    return 0;
}

int check()                // Check if a Corpus has been opened or created
{
    if (project.empty()) {
        cerr << "-Shuuseki: Caution: You may need to create a Corpus or open one" << endl;
        return -1;
    }
    return 0;
}

int import(const vector<string> &files)
{
    check();

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

    imported_corpus.close();

    cout << "-Shuuseki: You have been imported " + to_string(count) + " files " + "into the Corpus [" + Shuuseki.CorpusName + "] successfully" << endl;

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
        string tfile = transInput(file);
        int pos = match(Shuuseki.FileList, tfile);
        if (pos == -1) continue;

        map<string, map<string, vector<vector<int>>>>::iterator item;
        item = indexes.find(tfile);

        if (item != indexes.end())
            indexes.erase(tfile);

        Shuuseki.FileList.erase(Shuuseki.FileList.begin() + pos);
        ++count;
    }

    // Write the left content into corpus
    if (indexes.size() != record) {
        fstream updated_corpus(project + ".corpus", ios_base::out | ios_base::trunc);
        writeIndiceIntoCorpus(indexes, updated_corpus);
        updated_corpus.close();
    }

    cout << "-Shuuseki: You have delete " + to_string(count) + " files successfully from Corpus [" + Shuuseki.CorpusName + "]" << endl;

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
        if ((HFKaiji.find(lhsFirst) != HFKaiji.end()) && (HFKaiji.find(rhsFisrt) != HFKaiji.end()))
            return HFKaiji[lhsFirst] < HFKaiji[rhsFisrt];
        else
            return false;
    }
};

// Complex sort: 1) base on line_no; 2) base on character_no;
struct CompIntCmp {
    bool operator()(const pair<string, vector<int>> &lhs, const pair<string, vector<int>> &rhs)
    {
        if (lhs.second[0] < rhs.second[0])
            return true;
        else if (lhs.second[0] == rhs.second[0])
            return lhs.second[1] < rhs.second[1];

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

    cout << "-Shuuseki: In Corpus [" + Shuuseki.CorpusName + "], there are " + to_string(Shuuseki.FileList.size()) + " imported files, "
         << "almost " + to_string(charactersNum) + " characters(Kanji or English words)." << endl;

    cout << " And later you will see the Kanji with top 20 occurrences. Please wait for a minute......(only if your Corpus is big)" << endl;

    // Count the occurrences of characters
    map<string, int> wordOccurrences;
    countOccurrence(indexes, wordOccurrences);

    // Sort the map

    vector<pair<string, int>> result(wordOccurrences.begin(), wordOccurrences.end());

    sort(result.begin(), result.end(), IntCmp());

    for (int i = 0; i < 20; ++i) {
        cout << " " << transOutput(result[i].first) << " occurs " << result[i].second << ((result[i].second > 1) ? " times" : " time") << endl;
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

    // Get characters from indexes and rebuild into sentences
    vector<string> content;
    vector<pair<string, vector<int>>> sentences;
    map<string, map<string, vector<vector<int>>>>::iterator item;
    for (item = indexes.begin(); item != indexes.end(); ++item) {
        vector<pair<string, vector<int>>> kanji;
        for (auto & c : item -> second)
            for (auto &i : c.second)
                kanji.emplace_back(c.first, i);

        sort(kanji.begin(), kanji.end(), CompIntCmp());
        sentences.insert(sentences.end(), kanji.begin(), kanji.end());
    }

    content.reserve(sentences.size());
    for (auto &sentence : sentences)
        content.push_back(sentence.first);

    // n_gram string frequency statistic
    int n_min = stoi(options[1]), n_max = stoi(options[2]), f_min = stoi(options[3]), f_max = stoi(options[4]);

    map<string, int> stringOccurrences = n_gram(n_min, n_max, content, f_min, f_max);
    vector<pair<string, int>> temp(stringOccurrences.begin(), stringOccurrences.end()), result;

    // Reduce the number of elements which need to be sorted
    // And it make sense
    for (auto &t : temp)
        if (t.second >= f_min && t.second <= f_max)
            result.push_back(t);

    ofstream out("recurrence.txt", ios_base::out | ios_base::trunc);
    if (options[0] == "-f") {
        sort(result.begin(), result.end(), IntCmp());
    } else {
        sort(result.begin(), result.end(), AlpCmp());
    }

    out << " 频次" << "\t\t" << "字符串" << endl;

    for (auto &r : result) {
            out << " " << r.second << "\t\t" << r.first << endl;
    }

    out.close();

    cout << "-Shuuseki: Finished! You could read the result in recurrence.txt. " << endl;

    return 0;
}

// Just for search
string merge(vector<pair<string, vector<int>>> sentence, const int &start, const int &length)
{
    string s;
    for (int i = start; i < (length + start); ++i) {
        s += sentence[i].first;
    }
    return s;
}

string complete(string &serial)
{
    while (serial.size() < 7)
        serial = "0" + serial;

    return serial;
}

int search(vector<string> options)
{
    if (options.size() != 4) {
        cerr << "-Shuuseki: missing options. Please input command [sort] with correct options." << endl;
        return -1;
    }

    check();

    cout << "-Shuuseki: Please wait for a minute..." << endl;

    string source = transInput(options[0]), goal = transInput(options[3]);
    int rangeMin = stoi(options[1]), rangeMax =stoi(options[2]);

    vector<pair<string, vector<int>>> sentences;
    map<string, map<string, vector<vector<int>>>>::iterator item;

    vector<pair<int, string>> results;

    for (item = indexes.begin(); item != indexes.end(); ++item) {
        vector<pair<string, vector<int>>> kanji;
        vector<vector<int>> anchor;
        for (auto & c : item -> second)
            for (auto &i : c.second)
                kanji.emplace_back(c.first, i);

        sort(kanji.begin(), kanji.end(), CompIntCmp());

        for (auto &it : kanji) {
            if (it.first == source)
                anchor.push_back({it.second[0], it.second[1]});
        }

        for (auto &a : anchor) {
            vector<pair<string, vector<int>>> sentence;
            for (auto &k : kanji) {
                if (k.second[0] == a[0])
                    sentence.push_back(k);
            }

            // Expand the length of the sentence
            int end = a[1] + rangeMax + 1, start = a[1] + rangeMin + 1, increment = 1;
            while (end > sentence.size()) {
                for (auto &k : kanji) {
                    if (k.second[0] == (a[0] + increment))
                        sentence.push_back(k);
                }
                ++increment;
            }

            while (start <= end) {
                if (sentence[start].first == goal) {
                    string front = merge(sentence, 0, a[1]),
                           middle = merge(sentence, a[1], start + 1 - a[1]),
                           last = merge(sentence, start + 1, static_cast<int>(sentence.size() - start - 1));
                    stringstream ss;
                    ss << " from [" << (item -> first) << "]: "<< front << " *" << middle << "* " << last;
                    results.emplace_back(a[0], ss.str());
                    ++start;
                } else {
                    ++start;
                }
            }
        }
    }

    ofstream out("results.txt", ios_base::out | ios_base::trunc);
    out << "-------------------------------------" << endl;
    if (!results.empty())
        out << "+ " << results.size() << " sentences found and listed blow:" << endl;
    else
        out << "404 Not Found" << endl;
    out << "-------------------------------------" << '\n' <<endl;

    for (auto &r : results) {
        string serial = to_string(r.first);
        if (serial.size() < 7)
            complete(serial);

        out << "line " << serial << r.second << endl;
    }

    cout << "-Shuuseki: Finished! You could read the result in result.txt. " << endl;

    return 0;
}
