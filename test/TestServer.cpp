#include "TestServer.hpp"

#include "SensorHandler.hpp"
#include "TestSensor.hpp"
#include "FileStream.hpp"
#include <unistd.h>

TestServer::TestServer(std::istream* in, std::ostream* out) {
    stream = new FileStream(in, out);
    wrapper.setStream(stream);
    wrapper.setSensors(&sensors);
}

TestServer::~TestServer() {
    delete stream;
}

void TestServer::handle() {
    wrapper.handle();
}
