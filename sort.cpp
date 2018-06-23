//
// Created by Hybin on 2018/6/17.
//

#include <iostream>
#include <regex>
#include "include/Corpus.h"

using namespace std;

vector<string> getStringFromCorpus(fstream &in)
{
    string line;
    vector<string> lines;

    while (getline(in, line)) {
        if (!line.empty()) {
            lines.emplace_back(line.c_str());
        } else {
            continue;
        }
    }
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
    string hex = string_to_hex(s), word, dora, cnDora;  // dora means digit or alphabet.
    while (!hex.empty()) {
        bytes.push_back(hex.substr(0, 2));
        hex = hex.substr(2, hex.size() - 2);
    }

    set<string> SpeS = {"2e", "3a", "2c", "22", "3f", "2d", "21", "27", "60", "3c", "3e", "5b", "5d", "7b", "7d", "56", "5f",
                        "40", "23", "24", "25", "5e", "26", "2a", "28", "29", "2b", "3d", "2f", "5c", "7c", "7e", "3b", "57",
                        "efbc88", "efbc89", "efb999", "efb99a", "efbc8c", "e38081", "e38082", "efbc8e", "efbc9b", "efbc9a",
                        "efbc9f", "efbc81", "e3808a", "e3808b", "e28098", "e28099", "e2809c", "e2809d", "e29480", "e280a6",
                        "e38080", "e38090", "e38091", "efbc8d", "efb9a3", "e28094", "20"},
                cNda = {"efbca1", "efbca2", "efbca3", "efbca4", "efbca5", "efbca6", "efbca7", "efbca8", "efbca9", "efbcaa", "efbcab", "efbcac", "efbcad",
                        "efbcae", "efbcaf", "efbcb0", "efbcb1", "efbcb2", "efbcb3", "efbcb4", "efbcb5", "efbcb6", "efbcb7", "efbcb8", "efbcb9", "efbcba",
                        "efbd81", "efbd82", "efbd83", "efbd84", "efbd85", "efbd86", "efbd87", "efbd88", "efbd89", "efbd8a", "efbd8b", "efbd8c", "efbd8d",
                        "efbd8e", "efbd8f", "efbd90", "efbd91", "efbd92", "efbd93", "efbd94", "efbd95", "efbd96", "efbd97", "efbd98", "efbd99", "efbd9a",
                        "efbc90", "efbc91", "efbc92", "efbc93", "efbc94", "efbc95", "efbc96", "efbc97", "efbc98", "efbc99"};

    int i = 0;
    while (i < bytes.size()) {
        // Check if it is a Kanji
        if (regex_match(bytes[i], regex("(e)([0-9a-f])"))) {
            word += bytes[i] + bytes[i + 1] + bytes[i + 2];
            i += 3;
            clear(dora, words);
            if (SpeS.find(word) != SpeS.end()) word.clear();
            if (cNda.find(word) != cNda.end()) {
                cnDora += word;
                word.clear();
                continue;
            } else {
                clear(cnDora, words);
            }
            clear(word, words);
            continue;
        }
        // Check if it is an English word or digits
        if (regex_match(bytes[i], regex("([2-7])([0-9a-f])"))) {
            if (SpeS.find(bytes[i]) == SpeS.end()) dora += bytes[i];

            clear(word, words);
            clear(cnDora, words);
            i += 1;
            continue;
        }
        // Check if it is an Greek or Cyrillic alphabet
        if (regex_match(bytes[i], regex("([c-d])([0-9a-f])"))) {
            i += 2;
            clear(word, words);
            clear(dora, words);
            clear(cnDora, words);
            continue;
        }
    }
    // If the last word is an English word, we may need this step
    clear(dora, words);

    return words;
}

