#include "MemoryStream.h"

MemoryStream::MemoryStream(char* i, char* o, const size_t l)
{
    len = l;
    in = i;
    out = o;
    iptr = optr = 0;
}
MemoryStream::~MemoryStream() {
}

int MemoryStream::available()
{
    return len-iptr;
}

size_t MemoryStream::readBytes(char *buff, size_t l)
{
    size_t a = l>len-iptr?len-iptr:l;
    memcpy(buff,in+iptr, a);
    iptr += a;
    return a;
}

size_t MemoryStream::write(const uint8_t *buff, size_t l)
{
    size_t a = l>len-optr?len-optr:l;
    memcpy(out+optr,buff, a);
    optr += a;
    return a;
}
