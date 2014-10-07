#ifndef FILESTREAM_H
#define FILESTREAM_H

#include "compat.hpp"
#include <istream>
#include <ostream>

class FileStream : public Stream
{
public:
    FileStream(std::istream* , std::ostream*);
    ~FileStream();
    int available();
    size_t readBytes( char *, size_t );
    size_t write(const uint8_t *, size_t );
private:
    std::istream* in;
    std::ostream* out;
};

#endif // FILESTREAM_H
