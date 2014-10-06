#include "TestServer.hpp"

#include <iostream>
#include <stdio.h>
#include <unistd.h>

TestServer::TestServer(FILE* in, FILE* out) {
    stream = new FileStream(in, out);
    ProtocolWrapper::setStream(stream);

    sensors = new sensor::Sensor*[1];
    sensors[0] = new TestSensor("TestSensor", 128L,8L);
    detector = new SensorDetector(sensors, 1);
    wrapper = new ProtocolWrapper(detector);
}

TestServer::~TestServer() {
    delete wrapper;
    delete detector;
    delete sensors;
    delete stream;
}

void TestServer::handle() {
    if (wrapper != NULL)
        wrapper->handle();
}

int main(int argc, char **argv) {
    FILE* in = stdin;
    FILE* out = stdout;

    if (argc >= 2) {
        in = fopen(argv[1], "rb");
    }

    if (argc >= 3) {
        out = fopen(argv[2], "wb");
    }

    TestServer server(in, out);
    server.handle();

    usleep(10000);

    fclose(in);
    fclose(out);

    exit(0);
}
