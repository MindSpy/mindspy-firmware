#ifndef FILESTREAM_H
#define FILESTREAM_H

#include "compat.h"
#include <stdio.h>

class FileStream : public Stream
{
public:
    FileStream(FILE*, FILE*);
    ~FileStream();
    int available();
    size_t readBytes( char *, size_t );
    size_t write(const uint8_t *, size_t );
private:
    FILE* in;
    FILE* out;
};

#endif // FILESTREAM_H
