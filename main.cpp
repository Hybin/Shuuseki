#include <iostream>
#include <cstring>
#include "include/Corpus.h"

using namespace std;

int main()
{
    /*
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
            if (import(files) == -1) continue;
        }

        if (corpusCmd == "delete") {
            if (remove(files) == -1) continue;
        }

        if (corpusCmd == "corpus") {
            scCorpusCmd = s.substr(mark + 1, s.size() - mark - 1);
            if (scCorpusCmd == "status") {
                if (show(corpusName) == -1) continue;
            } else {
                cout << "-Shuuseki: " + scCorpusCmd + ": command not find" << endl;
            }
        }

    }
     */
    string s = "“苏小妹”是同船男学生为苏小姐起的个号。“东坡”2个字给鲍小姐南洋口音念得好像4444法国话里的“坟墓”（ｔｏｍｂｅａｕ）。苏小姐gen鲍小姐同舱，睡的是xia铺，比鲍小姐方便得多，";
    map<string, int> wordOccurrence;
    cout << countOccurence(s, wordOccurrence).size() << endl;


    return 0;
}