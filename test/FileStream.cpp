#include "FileStream.hpp"

FileStream::FileStream(std::istream*i , std::ostream*o ) : in(i), out(o) { }

FileStream::~FileStream() { }

int FileStream::available() {
    return SIZE_MAX;
}

size_t FileStream::readBytes( char *buff, size_t l) {
    return in->read(buff, l).gcount();
}

size_t FileStream::write(const uint8_t *buff, size_t l) {
    out->write((char*)buff, l);
    return l;
}
