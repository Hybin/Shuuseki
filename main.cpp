#include <iostream>
#include "include/Corpus.h"

using namespace std;



int main()
{
    string s;
    cout << ">>> ";
    cin >> s;
    cout << transform(s) << endl;

    return 0;
}