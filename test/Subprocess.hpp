#ifndef SUBPROCESS_HPP
#define SUBPROCESS_HPP

#include <iostream>
#include <ext/stdio_filebuf.h>

typedef int pfunc_t (int rfd, int wfd);


namespace mindspy {
namespace test {

class Subprocess
{
public:
    Subprocess(const char* fn, char*const* args);
    Subprocess(const char* fn);
    ~Subprocess();
    std::istream* istream();
    std::ostream* ostream();
    pid_t pid();
private:
    int fds[2];
    pid_t _pid;

    std::istream* in;
    std::ostream* out;
    __gnu_cxx::stdio_filebuf<char>* ibuf;
    __gnu_cxx::stdio_filebuf<char>* obuf;

    pid_t pcreate(const char* fn, char*const*);
    void setStreams();
};

}
} // namespace

#endif // SUBPROCESS_HPP
