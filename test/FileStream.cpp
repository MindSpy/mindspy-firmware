#include "FileStream.hpp"
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>


namespace mindspy {
namespace test {

FileStream::FileStream(const char*i , const char*o ) {
    if (i == NULL) {
        infd =  fileno(stdin);
    } else {
        infd = fileno(fopen(i, "rb"));
    }
    fcntl(infd, F_SETFL, fcntl(infd, F_GETFL) | O_NONBLOCK);

    if (o == NULL) {
        outfd = fileno(stdout);
    } else {
        outfd = fileno(fopen(o, "wb"));
    }
}

FileStream::~FileStream() {
    close(infd);
    close(outfd);
}

void FileStream::rdsync() {
    char buff;
    for (ssize_t r = -1; ; )
    {
        r = read(infd, &buff, 1);
        // TODO test errors
        r = rdbuf.sputn(&buff, r);
        if (r != 1)
            break;
    }
}

int FileStream::available() {
    rdsync();
    return rdbuf.in_avail();
}

size_t FileStream::readBytes( char *buff, size_t l) {
    int avail = available();
    return rdbuf.sgetn(buff, l>avail?avail:l);
}

size_t FileStream::write(const uint8_t *buff, size_t l) {
    ssize_t size = ::write(outfd, buff, l);
    fsync(outfd);
    return size;
}

}
} // namespace
