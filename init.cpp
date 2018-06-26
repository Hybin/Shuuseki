//
// Created by Hybin on 2018/6/9.
//

#include <iostream>
#include <functional>
#include <cctype>
#include <locale>
#include "include/Corpus.h"

using namespace std;

int initialize()
{
    ifstream in;
    in.open("corpora.index");
    if (!in) {
        fstream init("corpora.index", ios_base::in | ios_base::out | ios_base::app);
        init << "The Corpus You have built: \n";
    }
    in.close();
    return 0;
}

bool check(const string &s)
{
    // Read the data from the corpora.index
    vector<string> corpora;
    string line;

    ifstream init;
    init.open("corpora.index", ios_base::in);

    while (getline(init, line)) {
        corpora.push_back(line);
    }

    init.close();

    // Clean the array 'corpora'
    corpora.erase(corpora.begin());

    // Iterate the array and check;
    for (auto &i : corpora) {
        if (s == i) {
            return true;
        }
    }

    return false;
}

vector<string> split(const string &s, string token)
{
    string::size_type index;
    vector<string> result;
    string ss = string(s, 0, string::npos);
    while (!ss.empty()) {
        index = ss.find(token);
        if (index != string::npos) {
            result.push_back(ss.substr(0, index));
            ss = ss.substr(index + token.size());
        }
        else
        {
            result.push_back(ss);
            ss = "";
        }
    }
    return result;
}

int match(vector<string> &src, const string &s)
{
    vector<string>::iterator goal = find(src.begin(), src.end(), s);
    if (goal != src.end())
        return static_cast<int>(goal - src.begin());
    else
        return -1;
}

// Well, 3 function blow just erase the '\0' after the line, NOT spaces(in Chinese)
// trim from start
string &ltrim(string &s) {
    s.erase(s.begin(), find_if(s.begin(), s.end(),
                               not1(std::ptr_fun<int, int>(isspace))));
    return s;
}

// trim from end
string &rtrim(string &s) {
    s.erase(find_if(s.rbegin(), s.rend(),
                    not1(ptr_fun<int, int>(isspace))).base(), s.end());
    return s;
}

// trim from both ends
string &trim(string &s) {
    return ltrim(rtrim(s));
}

string string_to_hex(const string& input)
{
    static const char* const lut = "0123456789abcdef";
    size_t len = input.length();

    string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}

string hex_to_string(const string& input)
{
    static const char* const lut = "0123456789abcdef";
    size_t len = input.length();
    if (len & 1) throw invalid_argument("odd length");

    string output;
    output.reserve(len / 2);
    for (size_t i = 0; i < len; i += 2)
    {
        char a = input[i];
        const char* p = lower_bound(lut, lut + 16, a);
        if (*p != a) throw invalid_argument("not a hex digit");

        char b = input[i + 1];
        const char* q = lower_bound(lut, lut + 16, b);
        if (*q != b) throw invalid_argument("not a hex digit");

        output.push_back(((p - lut) << 4) | (q - lut));
    }
    return output;
}

map<string, map<string, vector<vector<int>>>> preprocessing(fstream &in, const string &file)
{
    map<string, map<string, vector<vector<int>>>> inventory;

    // Read the characters from file
    using line_no = int;
    vector<string> lines = getStringFromCorpus(in), sentences = removeEmptyLines(lines);
    for (int i = 0; i < sentences.size(); ++i) {
        vector<string> words = splitSentence(sentences[i]);
        for (int j = 0; j < words.size(); ++j)
            inventory[file][words[j]].push_back({i, j});    // filename - kanji - line_no - char_no
    }

    return inventory;
}

int writeIndiceIntoCorpus(map<string, map<string, vector<vector<int>>>> &inventory, fstream &out)
{
    map<string, map<string, vector<vector<int>>>>::iterator item;

    for (item = inventory.begin(); item != inventory.end(); ++item)
        for (auto & c : item -> second)
            for (auto &i : c.second)
                out << item -> first << "-" << c.first << "-" << i[0] << "-" << i[1] << endl;

    return 0;
}

map<string, map<string, vector<vector<int>>>> getDataFromCorpus(fstream &in)
{
    map<string, map<string, vector<vector<int>>>> inventory;
    vector<string> indexes = getStringFromCorpus(in);

    for (auto &index : indexes) {
        vector<string> record = split(index, "-");
        inventory[record[0]][record[1]].push_back({stoi(record[2]), stoi(record[3])});
    }

    return inventory;
};

int countOccurrence(map<string, map<string, vector<vector<int>>>> inventory, map<string, int> &wordOccurrences)
{
    map<string, map<string, vector<vector<int>>>>::iterator item;
    for (item = inventory.begin(); item != inventory.end(); ++item)
        for (auto & c : item -> second)
            for (auto &i : c.second)
                ++wordOccurrences[c.first];

    return 0;
}
