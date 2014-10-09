#include "ProtocolTest.hpp"

#include "compat.hpp"
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include "Subprocess.hpp"
#include "TestServer.hpp"

#include <google/protobuf/io/zero_copy_stream_impl.h>

using namespace google::protobuf;

bool ProtocolTest::writeDelimitedTo(const MessageLite* message, io::ZeroCopyOutputStream* rawOutput) {
    // We create a new coded stream for each message.  Don't worry, this is fast.
    io::CodedOutputStream output(rawOutput);

    // Write the size.
    const uint64_t size = message->ByteSize();
    output.WriteVarint64(size);

    message->SerializeToCodedStream(&output);
    if (output.HadError())
        return false;

    return true;
}

bool ProtocolTest::readDelimitedFrom(io::ZeroCopyInputStream* rawInput, MessageLite* message) {
  // We create a new coded stream for each message.  Don't worry, this is fast,
  // and it makes sure the 64MB total size limit is imposed per-message rather
  // than on the whole stream.  (See the CodedInputStream interface for more
  // info on this limit.)
  io::CodedInputStream input(rawInput);

  // Read the size.
  uint64_t size;
  if (!input.ReadVarint64(&size))
      return false;

  // Tell the stream not to read beyond that size.
  io::CodedInputStream::Limit limit = input.PushLimit(size);

  // Parse the message.
  if (!message->MergeFromCodedStream(&input))
      return false;
  if (!input.ConsumedEntireMessage())
      return false;

  // Release the limit.
  input.PopLimit(limit);

  return true;
}

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

    protobufs::Request req;
    protobufs::Response res;

    req.Clear();
    res.Clear();

    req.set_timestamp(timestamp());
    req.set_reqid(reqid());
    //req.set_has_getmodelname();

    Subprocess sub("server");

    io::ZeroCopyInputStream* is = new io::IstreamInputStream(sub.istream());
    io::ZeroCopyOutputStream* os = new io::OstreamOutputStream(sub.ostream());

    TEST_ASSERT_MSG((!writeDelimitedTo(&req, os)), "Write to stream failed.");
    TEST_ASSERT_MSG((!readDelimitedFrom(is, &res)), "Read from stream failed.");
}

void ProtocolTest::testHandle() {

    protobufs::Request req;
    protobufs::Response res;

    std::stringstream cosi;
    std::stringstream ciso;

    TestServer server(&cosi, &ciso);

    req.Clear();
    res.Clear();

    req.set_timestamp(timestamp());
    req.set_reqid(reqid());
    //req.set_has_getmodelname();

    io::ZeroCopyInputStream* is = new io::IstreamInputStream(&ciso);
    io::ZeroCopyOutputStream* os = new io::OstreamOutputStream(&cosi);

    TEST_ASSERT_MSG((!writeDelimitedTo(&req, os)), "Write to stream failed.");
    server.handle();
    TEST_ASSERT_MSG((!readDelimitedFrom(is, &res)), "Read from stream failed.");

    // TODO check res
}
