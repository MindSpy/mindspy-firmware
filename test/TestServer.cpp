#include "TestServer.hpp"

#include "SensorHandler.hpp"
#include "TestSensor.hpp"
#include "FileStream.hpp"
#include <unistd.h>
#include <fstream>

TestServer::TestServer(std::istream* in, std::ostream* out) {
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
