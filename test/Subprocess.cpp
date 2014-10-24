#include "Subprocess.hpp"

#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <ext/stdio_filebuf.h>

namespace mindspy {
namespace test {

Subprocess::Subprocess(const char* fn, char*const* args) {
    _pid = pcreate(fn, args);
    setStreams();
}

Subprocess::Subprocess(const char* fn) {
    _pid = pcreate(fn, new char*[0]{});
    setStreams();
}

void Subprocess::setStreams() {
    // create filebuffers of file descriptor
    ibuf = new __gnu_cxx::stdio_filebuf<char>(fds[0], std::ios::in);
    obuf = new __gnu_cxx::stdio_filebuf<char>(fds[1], std::ios::out);
    // create istreams of filebuffers
    in = new std::istream(ibuf);
    out = new std::ostream(obuf);
}

Subprocess::~Subprocess() {
    delete in;
    delete out;
    delete ibuf;
    delete obuf;
}

std::istream* Subprocess::istream() {
    return in;
}

std::ostream* Subprocess::ostream() {
    return out;
}

pid_t Subprocess::pid() {
    return _pid;
}

pid_t Subprocess::pcreate(const char* fn, char*const* args) {
    /* Spawn a process from pfunc, returning it's pid. The fds array passed will
     * be filled with two descriptors: fds[0] will read from the child process,
     * and fds[1] will write to it.
     * Similarly, the child process will receive a reading/writing fd set (in
     * that same order) as arguments.
    */
    pid_t pid;
    int pipes[4];

    /* Warning: I'm not handling possible errors in pipe/fork */

    if (pipe(&pipes[0]) < 0) /* Parent read/child write pipe */
        exit(1); // ERROR

    if (pipe(&pipes[2]) < 0) /* Child read/parent write pipe */
        exit(1); // ERROR

    if ((pid = fork()) > 0) {
        /* Parent process */
        fds[0] = pipes[0];
        fds[1] = pipes[3];

        close(pipes[1]);
        close(pipes[2]);

        return pid;

    } else {
        close(pipes[0]);
        close(pipes[3]);

        // duplicate file descriptors and spawn process
        if (dup2( STDIN_FILENO, pipes[2] ) < 0)
            exit(1); // ERROR

        if (dup2( STDOUT_FILENO, pipes[1] ) < 0)
            exit(1); // ERROR

        if (execv( fn, args ) < 0)
            exit(1); // ERROR

        exit(0);
    }

    return -1; /* ? */
}

}
} // namespace
