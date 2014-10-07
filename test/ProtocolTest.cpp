#include "ProtocolTest.hpp"

#include "Proto.hpp"
#include "compat.hpp"
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include "Subprocess.hpp"

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
    Request* request = (Request*) req;
    Response* response = (Response*) res;


    Subprocess sub("server");

    //pid_t pid = sub.pid();
    std::istream* in = sub.istream();
    std::ostream* out = sub.ostream();

    request->SerializePartialToOstream(out);
    response->ParseFromIstream(in);
}

void ProtocolTest::testLocal() {
}

void ProtocolTest::testRemote() {
}

void ProtocolTest::testHandle() {

    Request req;
    Response res;

    req.Clear();
    req.set_timestamp(timestamp());
    req.set_reqid(reqid());
    //req.set_has_getmodelname();

    communicate(&req, &res);

}
