#include "FileStream.hpp"

FileStream::FileStream(FILE*i, FILE*o) : in(i), out(o) { }

FileStream::~FileStream() { }

int FileStream::available() {
    return SIZE_MAX;
}

size_t FileStream::readBytes( char *buff, size_t l) {
    return fread(buff, 1, l, in);
}

size_t FileStream::write(const uint8_t *buff, size_t l) {
    return fwrite(buff, 1, l, out);
}
