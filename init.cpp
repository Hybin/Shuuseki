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

// Trying to delete all spaces in Chinese(0x20)
string &atrim(string &s)
{
    string res;
    for (char &i : s) {
        if (i != 0x20 && !isPuncMark(i))
            res += i;
    }
    return res;
}

bool isPuncMark(const char &c)
{
    // .2e :3a ,2c "22 ?3f -2d !21 '27 `60 <3c >3e @40 #23 $24 =3d
    // [5b ]5d {7b }7d /2f \5c |7c ~7e %25 ^5e &26 *2a (28 )29 +2b
    vector<int> mark = {0x2e, 0x3a, 0x2c, 0x22, 0x3f, 0x2d, 0x21, 0x27, 0x60, 0x3c, 0x3e, 0x5b, 0x5d, 0x7b, 0x7d,
                        0x40, 0x23, 0x24, 0x25, 0x5e, 0x26, 0x2a, 0x28, 0x29, 0x2b, 0x3d, 0x2f, 0x5c, 0x7c, 0x7e};
    for (auto &m : mark) {
        if (m == c) return true;
    }
    return false;
}

bool isDigitOrAlphabet(const char &c)
{
    vector<int> digitOrAlphabets = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
                                    0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a,
                                    0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74,
                                    0x75, 0x76, 0x77, 0x78, 0x79, 0x7a};
    for (auto &d : digitOrAlphabets) {
        if (d == c) return true;
    }
    return false;
}

vector<int> getLenOfNonKanji(vector<int> &m)
{
    vector<int> n, res;
    for (int i = 0; i < m.size(); ++i) {
        if (i == 0) {
            n.push_back(m[i]);
        } else {
            if (m[i] != m[i - 1]) {
                n.push_back(m[i]);
            }
        }
    }

    for (int &j : n) {
        if (j != 0) {
            res.push_back(j);
        }
    }
    return res;
}

int countKanji(string &s)
{
    const char *line = atrim(trim(s)).c_str();
    vector<int> characters;

    int lenOfDigitOrAlphabet, sizeOfLine = static_cast<int>(string(line).size()), lenOfLine;

    for (char &i : string(line)) {
        if (isDigitOrAlphabet(i)) {
            sizeOfLine -= 1;
            characters.push_back(1);
        } else {
            characters.push_back(0);
        }
    }

    lenOfDigitOrAlphabet = static_cast<int>(getLenOfNonKanji(characters).size());

    lenOfLine = (sizeOfLine / 3) + lenOfDigitOrAlphabet;

    return lenOfLine;
}



