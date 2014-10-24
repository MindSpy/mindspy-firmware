#ifndef FILESTREAM_H
#define FILESTREAM_H

#include "compat.hpp"
#include <cstdio>
#include <sstream>

class FileStream : public Stream
{
public:
    FileStream(const char* , const char*);
    ~FileStream();
    int available();
    size_t readBytes( char *, size_t );
    size_t write(const uint8_t *, size_t );
private:
    int infd;
    int outfd;
    std::stringbuf rdbuf;

    void rdsync();
};

#endif // FILESTREAM_H
