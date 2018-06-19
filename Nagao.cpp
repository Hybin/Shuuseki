//
// Created by Hybin on 2018/6/17.
//

#include <iostream>
#include "include/Nagao.h"

using namespace std;

vector<string> getStingFromCorpus(string &corpus)
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

vector<string> removeEmptyLines(vector<string> &lines)
{
    vector<string> sentences;
    for (auto &i : lines) {
        if (i.size() > 1)
            sentences.push_back(trim(i));
    }
    cout << sentences.size() << endl;
    return sentences;
}

