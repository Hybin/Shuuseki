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

vector<string> readCorpusInfo(ifstream &in) {
    string info;
    vector<string> inventory;

    while (in >> info) {
        inventory.push_back(info);
    }

    return inventory;
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

string vector2string(vector<string> vec)
{
    string s;
    for (auto &i : vec) {
        s += i + ",";
    }

    return s;
}

vector<string> getAbsIndice(vector<string> &src)
{

    auto fileNum = static_cast<int>(src.size() / 11);      // Get the number of files you have imported
    int iter = (fileNum - 1) * 11;

    vector<string> absIndex;                                 // Get a list only with file names and a list including file names and indice
    for (int i = 1; i <= (iter + 1); i += 11) {
        absIndex.push_back(src[i]);                       // Get abstract Index
    }

    return absIndex;
}

vector<vector<string>> getConIndice(vector<string> &src)
{
    auto fileNum = static_cast<int>(src.size() / 11);      // Get the number of files you have imported
    int iter = (fileNum - 1) * 11;

    vector<vector<string>> conIndex;                         // Get a list only with file names and a list including file names and indice

    for (int i = 1; i <= (iter + 1); i += 11) {
        vector<string> atom;

        atom.push_back(src[i]);                           // Get the file name
        atom.push_back(src[i + 3]);                       // Get the index:begin
        atom.push_back(src[i + 6]);                       // Get the index:end
        atom.push_back(src[i + 9]);                       // Get the word count

        conIndex.push_back(atom);                            // Get concrete Index
    }

    return conIndex;
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

unsigned long countKanji(vector<string> &kanji)
{
    return kanji.size();
}

int countOccurrence(vector<string> &kanji, map<string, int> &wordOccurrences)
{
    for (auto &i : kanji)
        ++wordOccurrences[i];

    return 0;
}

