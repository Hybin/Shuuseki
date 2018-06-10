//
// Created by Hybin on 2018/6/10.
//

#include <sstream>
#include <iterator>
#include <iostream>
#include "include/Corpus.h"

using namespace std;

// Check the encoding of the file (GB* or Big 5)
string is_gb_or_big5(const char * file)
{
    FILE * in;
    in = fopen(file, "r");
    int i, hanzinum = 0, hanzibyte = 0;

    while ((i = fgetc(in)) != EOF) {
        if (i & 0x80) {
            hanzibyte += i;
            hanzinum++;
        }
    }
    fclose(in);

    float ratio = (float)hanzibyte / (float)hanzinum;
    string encode;
    if (ratio > 184)
        encode = "GB码";
    else if (ratio > 180)
        encode = "UTF-8";
    else
        encode = "Big5";

    return encode;
}

// Check the encoding of the file (Unicode * or UTF-8)
string is_unicode_or_utf8(string &file)
{
    ifstream in(file, ios_base::binary);
    unsigned char c;
    in.read((char*) &c, sizeof(c));
    int p = c << 8;
    in.read((char*) &c, sizeof(c));
    p += c;

    string encode;

    switch (p) {
        case 0xfffe:
            encode = "UTF-16LE";
            break;
        case 0xfeff:
            encode = "UTF-16BE";
            break;
        case 0xefbb:
            encode = "UTF-8";
            break;
        default:
            encode = "other";
    }

    in.close();
    return encode;
}

string checkEncoding(std::string &file)
{
    string encode = is_unicode_or_utf8(file);
    const char * cfile = file.c_str();
    if (encode == "other") encode = is_gb_or_big5(cfile);

    return encode;
}



