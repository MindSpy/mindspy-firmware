
#include "TestServer.hpp"

#include <fstream>
#include <unistd.h>

int main(int argc, char **argv) {
    std::ifstream* in = NULL;
    std::ofstream* out = NULL;

    if (argc >= 2) {
        in = new std::ifstream(argv[1]);
    }

    if (argc >= 3) {
        out = new std::ofstream(argv[2], std::ios::trunc);
    }

    TestServer server(in==NULL?&std::cin:in, out==NULL?&std::cout:out);
    server.handle();

    usleep(10000);

    if (in!=NULL)
        delete in;
    if (out!=NULL)
        delete out;

    exit(0);
}
