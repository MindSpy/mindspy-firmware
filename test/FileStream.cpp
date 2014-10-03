#include "FileStream.h"

FileStream::FileStream(FILE*i, FILE*o)
{
    in = i;
    out = o;
}
int FileStream::available()
{
    return SIZE_MAX;
}

size_t FileStream::readBytes( char *buff, size_t l)
{
    return fread(buff, l, 1, in);
}

size_t FileStream::write(const uint8_t *buff, size_t l)
{
    return fwrite(buff, l, 1, out);
}
