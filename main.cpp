#include <iostream>
#include "include/Corpus.h"

using namespace std;



int main()
{
    string s;
    cout << ">>> ";
    cin >> s;
    fstream test(s, ios_base::in | ios_base::out);
    string t;
    vector<string> r;
    while (test >> t) {
        r.push_back(t);
    }
    cout << r[3] << " ";
    cout << r[8];

    return 0;
}