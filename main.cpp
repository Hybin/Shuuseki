#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "include/Corpus.h"

using namespace std;

int main()
{
    initialize();

    string s;
    cout << ">>> ";
    getline(cin, s);
    create(s);
    return 0;
}