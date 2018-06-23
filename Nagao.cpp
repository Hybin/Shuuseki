//
// Created by Hybin on 2018/6/17.
//

#include <iostream>
#include <regex>
#include "include/Nagao.h"

using namespace std;

vector<string> getStringFromCorpus(string &corpus)
{
    ifstream in(corpus);
    string line;
    vector<string> lines;

    while (getline(in, line)) {
        if (!line.empty()) {
            lines.emplace_back(line.c_str());
        } else {
            continue;
        }
    }
    cout << lines.size() << endl;
    in.close();
    return lines;
}

// Delete Lines filled with spaces
vector<string> removeEmptyLines(vector<string> &lines)
{
    vector<string> sentences;
    for (auto &i : lines) {
        if (trim(i).size() > 1)
            sentences.push_back(trim(i));
    }
    cout << sentences.size() << endl;
    return sentences;
}

// Split the sentences into vector<string>
int clear(string &s, vector<string> &vec) {
    if (!s.empty()) {
        vec.push_back(hex_to_string(s));
        s.clear();
    }
    return 0;
}
vector<string> splitSentence(const string &s)
{
    vector<string> words, bytes;
    string hex = string_to_hex(s), word, dora;  // dora means digit or alphabet.
    while (!hex.empty()) {
        bytes.push_back(hex.substr(0, 2));
        hex = hex.substr(2, hex.size() - 2);
    }

    int i = 0;
    while (i < bytes.size()) {
        // Check if it is a Kanji
        if (regex_match(bytes[i], regex("(e)([0-9a-f])"))) {
            word += bytes[i] + bytes[i + 1] + bytes[i + 2];
            i += 3;
            clear(dora, words);
        }
        // Check if it is an English word or digits
        if (regex_match(bytes[i], regex("([2-7])([0-9a-f])"))) {
            dora += bytes[i];
            i += 1;
            clear(word, words);
            continue;
        }
        // Check if it is an Greek or Cyrillic alphabet
        if (regex_match(bytes[i], regex("([c-d])([0-9a-f])"))) {
            i += 2;
            clear(word, words);
            clear(dora, words);
            continue;
        }
        // TODO: check if it is an Punctuation Mark or an English word or digits with UTF-8(Chinese Code)
        words.push_back(hex_to_string(word));
        word.clear();
    }
    // If the last word is an English word, we may need this step
    clear(dora, words);

    return words;
}

map<int, string> createPTable(const string &s)
{
    map<int, string> pTable;
    return pTable;
};
