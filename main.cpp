#include <iostream>
#include "include/Corpus.h"

using namespace std;

int main()
{
    initialize();

    string s;
    while (true) {
        cout << ">>> ";
        getline(cin, s);
        if (s == "exit") break;

        unsigned long mark = s.find(" "); // Find the pos of the 1st space
        if (mark == -1 && s != "exit") {
            cerr << "-Shuuseki: " + s + ": command not find" << endl;
            continue;
        }

        string corpusCmd = s.substr(0, mark), fileName, corpusName;
        vector<string> files;
        if (corpusCmd == "create" || corpusCmd == "open")
            corpusName = s.substr(mark + 1, s.size() - mark - 1);

        if (corpusCmd == "create") create(corpusName);
        if (corpusCmd == "open") openCorpus(corpusName);

        if (corpusCmd == "import" || corpusCmd == "delete") {
            fileName = s.substr(mark + 1, s.size() - mark - 1);
            files = split(fileName, " ");
        }

        if (corpusCmd == "import") {
            if (import(files) == -1) continue;
        }

        if (corpusCmd == "delete") {
            if (deleteCorpus(files) == -1) continue;
        }
    }

    return 0;
}