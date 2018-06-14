#include <iostream>
#include <cstring>
#include "include/Corpus.h"

using namespace std;

int main()
{
    initialize();

    cout << "Build your own corpus! Shuuseki is a small system written by C++ to help you manage your own corpora." << endl;
    cout << "You are able to use various of commands to manage your corpora, such like this: " << endl;
    cout << "   [create] [corpus name]" << endl;
    cout << "   [open] [corpus name]" << endl;
    cout << "   [import] [file name] [file name] [...]" << endl;
    cout << "   [delete] [file name] [file name] [...]" << "\n" <<endl;
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
    /*
    ifstream in("dream.txt", ios_base::binary | ios_base::ate);
    in.seekg(0, in.end);
    long long end_mark = in.tellg();
    in.seekg(0, ifstream::beg);
    auto *buffer = new char[end_mark];
    in.read(buffer, end_mark);
    cout << strlen(buffer);
    delete[] buffer;
    in.close();
     */
    return 0;
}