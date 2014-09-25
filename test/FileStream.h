#ifndef FILESTREAM_H
#define FILESTREAM_H

#include "ProtocolWrapper.h"

class FileStream : public Stream
{
public:
    FileStream();
    int available() = 0;
    size_t readBytes( char *, size_t ) = 0;
    size_t write(const uint8_t *, size_t );
};

#endif // FILESTREAM_H
