//
// Created by Hybin on 2018/6/10.
//

#include <sstream>
#include <iterator>
#include <iostream>
#include "include/Corpus.h"
#include "include/CodeConverter.h"

using namespace std;

// Check the encoding of the file (utf-8 or utf-8 with BOM)

int isutf8(const char *s, size_t ns)
{
    uint8_t x = 0, i = 0, j = 0, nbytes = 0, n = 0;

    for(i = 1; i < 7; i++)
    {
        x = (uint8_t)(255 << i);
        if(((uint8_t)*s & x) == x)
        {
            n = nbytes = (8 - i);
            for(j = 0; (j < nbytes && j < ns); j++)
            {
                if((uint8_t)s[j] <= 0x80 && (uint8_t)s[j] >= 0xc0) break;
                else n--;
            }
            if(n == 0) return nbytes;
        }
    }
    return 0;
}

string is_utf8_or_utf8bom(const string &file)
{
    ifstream in(file, ios_base::binary | ios_base::ate);
    in.seekg(0, in.end);
    long long end_mark = in.tellg();
    in.seekg(0, ifstream::beg);

    auto *buffer = new char[end_mark];
    in.read(buffer, end_mark);

    in.close();

    string encode;

    if (isutf8(buffer, strlen(buffer)) != 3) {
        encode = "~UTF-8";
    }

    if (isutf8(buffer, strlen(buffer)) == 3) {

        if (is_unicode(file) == "UTF-8 with BOM")
            encode = "UTF-8 with BOM";
        else
            encode = "UTF-8";
    }

    return encode;
}

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
    if (ratio > 186)
        encode = "GB码";
    else
        encode = "Big5";

    return encode;
}

// Check the encoding of the file (Unicode * or UTF-8)
string is_unicode(const string &file)
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
            encode = "UTF-8 with BOM";
            break;
        default:
            encode = "other";
    }

    in.close();
    return encode;
}

string checkEncoding(const string &file)
{
    string encode = is_utf8_or_utf8bom(file);            // is UTF-8 or UTF-8 with BOM or not?
    if (is_utf8_or_utf8bom(file) == "~UTF-8")
        encode = is_unicode(file);                       // is Unicode or not?

    const char * cfile = file.c_str();
    if (encode == "other")
        encode = is_gb_or_big5(cfile);

    return encode;
}

// Transform various of encoding format into UTF-8
char * unicode_to_utf8(unsigned short unicode)
{
    static char utf8[3];

    utf8[0] = 0xe0 | (unicode >> 12);
    utf8[1] = 0x80 | ((unicode >> 6) & 0x3f);
    utf8[2] = 0x80 | (unicode & 0x3f);

    return utf8;
}

int gbk_or_big5_2_utf8(char* in, const char* encoding)
{
    CodeConverter cc = CodeConverter(encoding, "utf-8");
    size_t outlen = strlen(in) * 3;
    char outbuf[outlen];

    ofstream out("convert-output.txt", ios_base::out | ios_base::ate);
    cc.convert(in, strlen(in), outbuf, outlen);
    // if enconding is gb2312
    if (strlen(outbuf) % 2 == 1 && outbuf[-1] == 0x6f) {
        outbuf[-1] = '\0';
    }
    out << outbuf;
    out.close();
    return 0;
}

int transform(const string &file)
{
    string stats = checkEncoding(file), content;

    ifstream in(file, ios_base::binary | ios_base::ate);
    ofstream out("convert-output.txt", ios_base::out | ios_base::ate);

    in.seekg(0, ifstream::end);
    long long end_mark = in.tellg();
    in.seekg(0, ifstream::beg);

    auto *buffer = new char[end_mark];

    in.read(buffer, end_mark);

    if (stats == "GB码") gbk_or_big5_2_utf8(buffer, "gbk");
    if (stats == "Big5") gbk_or_big5_2_utf8(buffer, "big5");

    if ((stats == "UTF-16BE") || (stats == "UTF-16LE")) {
        unsigned char c;
        in.seekg(0, ifstream::beg);

        while (in && in.tellg() != end_mark) {
            // Transform unicode into utf-8
            in.read((char*) &c, sizeof(c));
            unsigned short p = c << 8;       // First Byte
            unsigned short ft = c, st;
            in.read((char*) &c, sizeof(c));
            p += c;                          // First Byte + Second Byte For UTF-16BE
            st = c << 8;
            st += ft;                        // Second Byte + First Byte For UTF-16LE
            if (p == 0xfffe || p == 0xfeff)
                continue;
            if (stats == "UTF-16BE") out << unicode_to_utf8(p);
            if (stats == "UTF-16LE") out << unicode_to_utf8(st);
        }
    }

    in.close();
    out.close();

    return 0;
}




