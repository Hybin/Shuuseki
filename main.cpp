#include <iostream>
#include <cstring>
#include "include/Corpus.h"

using namespace std;
map<string, int> Htest = generate();

int main()
{
    initialize();

    cout << "Build your own corpus! Shuuseki is a small system written by C++ to help you manage your own corpora." << endl;
    cout << "You are able to use various of commands to manage your corpora, such like this: " << endl;
    cout << "   [create] [[corpus name]]" << endl;
    cout << "   [open]   [[corpus name]]" << endl;
    cout << "   [import] [[file name]] [[file name]] [[...]]" << endl;
    cout << "   [delete] [[file name]] [[file name]] [[...]]" << endl;
    cout << "   [corpus] [status]" << endl;
    cout << "To avoid Stack Overflow, if your file with encoding (~UTF-8), it is better to save it with encoding UTF-8 and import it." << "\n" <<endl;

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

        string corpusCmd = s.substr(0, mark), fileName, corpusName, scCorpusCmd;
        vector<string> files;
        if (corpusCmd == "create" || corpusCmd == "open")
            corpusName = s.substr(mark + 1, s.size() - mark - 1);

        if (corpusCmd == "create") create(corpusName);
        if (corpusCmd == "open") open(corpusName);

        if (corpusCmd == "import" || corpusCmd == "delete") {
            fileName = s.substr(mark + 1, s.size() - mark - 1);
            files = split(fileName, " ");
        }

        if (corpusCmd == "import") {
            cout << "-Shuuseki: Please wait for a minute..." << endl;
            if (import(files) == -1) continue;
        }

        if (corpusCmd == "delete") {
            cout << "-Shuuseki: Please wait for a minute..." << endl;
            if (remove(files) == -1) continue;
        }

        if (corpusCmd == "corpus") {
            scCorpusCmd = s.substr(mark + 1, s.size() - mark - 1);
            if (scCorpusCmd == "status") {
                cout << "-Shuuseki: Please wait for a minute..." << endl;
                if (show(corpusName) == -1) continue;
            } else {
                cout << "-Shuuseki: " + scCorpusCmd + ": command not find" << endl;
            }
        }

        if (corpusCmd == "sort") {
            vector<string> options = split(s.substr(mark + 1, s.size() - mark - 1), " ");
            cout << "-Shuuseki: Please wait for a minute..." << endl;
            if (sort(options) == -1) continue;
        }
    }

    return 0;
}