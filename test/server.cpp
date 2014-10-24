
#include "TestServer.hpp"

#include <cstdio>
#include <unistd.h>

int main(int argc, char **argv) {
    char *infn = NULL;
    char *outfn = NULL;

    if (argc >= 2) {
        infn = argv[1];
    }

    if (argc >= 3) {
        outfn = argv[2];
    }

    TestServer server(infn, outfn);

    while (1) {
        server.handle();
        //usleep(1000);
    }

    usleep(10000);

    exit(0);
}
