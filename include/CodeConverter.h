//
// Created by Hybin on 2018/6/12.
//

#ifndef SHUUSEKI_CODECONVERTER_H
#define SHUUSEKI_CODECONVERTER_H

#include <iconv.h>

class CodeConverter {
private:
    iconv_t cd;
public:
    // 构造
    CodeConverter(const char *from_charset,const char *to_charset) {
        cd = iconv_open(to_charset,from_charset);
    }

    // 析构
    ~CodeConverter() {
        iconv_close(cd);
    }

    // 转换输出
    int convert(char *inbuf,size_t inlen,char *outbuf,size_t outlen) {
        char **pin = &inbuf;
        char **pout = &outbuf;

        memset(outbuf,0,outlen);
        return static_cast<int>(iconv(cd, pin, &inlen, pout, &outlen));
    }
};

#endif //SHUUSEKI_CODECONVERTER_H
