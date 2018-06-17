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

    in.close();
    return lines;
}

vector<string> removeEmptyLines(vector<string> &lines)
{
    for (int i = 0; i < lines.size(); ++i) {
        if (lines[i].size() == 1)
            lines.erase(lines.begin() + (i - 1));
        else
            continue;
    }
    return lines;
}