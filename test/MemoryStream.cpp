#include "MemoryStream.hpp"

#include <cstring>


namespace mindspy {
namespace test {

MemoryStream::MemoryStream(char* i, char* o, const size_t il, const size_t ol) :
   olen(ol), ilen(il), in(i), out(o), iptr(0), optr(0) {}

MemoryStream::~MemoryStream() {
}

int MemoryStream::available()
{
    return ilen-iptr;
}

size_t MemoryStream::readBytes(char *buff, size_t l)
{
    size_t a = l>ilen-iptr?ilen-iptr:l;
    memcpy(buff,in+iptr, a);
    iptr += a;
    return a;
}

size_t MemoryStream::write(const uint8_t *buff, size_t l)
{
    size_t a = l>olen-optr?olen-optr:l;
    memcpy(out+optr,buff, a);
    optr += a;
    return a;
}

}
} // namespace
