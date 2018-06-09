//
// Created by Hybin on 2018/6/9.
//

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "include/Corpus.h"

using namespace std;

int initialize()
{
    ifstream in;
    in.open("init.index", ios_base::in);
    if (!in) {
        fstream init("init.index", ios_base::in | ios_base::out | ios_base::app);
        init << "The Corpus You have built: \n";
    }
    in.close();
    return 0;
}

bool check(const string &s)
{
    // Read the data from the init.index
    vector<string> corpora;
    string line;

    ifstream init;
    init.open("init.index", ios_base::in);

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