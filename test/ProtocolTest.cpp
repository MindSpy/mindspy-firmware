#include "ProtocolTest.hpp"

#include "Proto.hpp"
#include "compat.hpp"
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include "Subprocess.hpp"
#include "TestServer.hpp"

ProtocolTest::ProtocolTest()
{
    in = new std::stringstream;
    out = new std::stringstream;
    TEST_ADD(ProtocolTest::testHandle);
    TEST_ADD(ProtocolTest::testLocal);
    TEST_ADD(ProtocolTest::testRemote);
}

void ProtocolTest::setup()
{
}

void ProtocolTest::tear_down()
{
}

uint64_t ProtocolTest::timestamp() {
    return micros();
}

uint64_t ProtocolTest::reqid() {
    return rand();
}


void ProtocolTest::communicate(void* req, void* res) {
    protobufs::Request* request = (protobufs::Request*) req;
    protobufs::Response* response = (protobufs::Response*) res;

    Subprocess sub("server");

    //pid_t pid = sub.pid();
    std::istream* in = sub.istream();
    std::ostream* out = sub.ostream();

    // TODO read and write delimited
    //request->SerializeAsString()  ->SerializePartialToOstream(out);
    //response->ParseFromIstream(in);
}

void ProtocolTest::testLocal() {

}

void ProtocolTest::testRemote() {
}

void ProtocolTest::testHandle() {

    protobufs::Request req;
    protobufs::Response res;

    std::stringstream str1;
    std::stringstream str2;

    TestServer server(&str1, &str2);

    req.Clear();
    res.Clear();

    req.set_timestamp(timestamp());
    req.set_reqid(reqid());
    //req.set_has_getmodelname();

    req.SerializeToOstream(&str1);
    server.handle();
    res.ParseFromIstream(&str2);
}
