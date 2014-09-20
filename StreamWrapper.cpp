#include <pb_encode.h>
#include <pb_decode.h>
#include "StreamWrapper.h"
#include "macros.h"
#include "SensorHandler.h"
#include <Logging.h>

Stream*  StreamWrapper::_serial;

void StreamWrapper::init(Stream* serial, ISensor** sensors, size_t sensorCount) {
  _serial = serial;
  SensorHandler::init(&pb_decode_delimited, &pb_encode_delimited, (TimestampCallbackType)&micros, sensors, sensorCount);
}

void StreamWrapper::handle(void) {
  Request request;
  Response response;
  
  pb_istream_t istream = {&StreamWrapper::read_callback, StreamWrapper::_serial, SIZE_MAX};
  pb_ostream_t ostream = {&StreamWrapper::write_callback, StreamWrapper::_serial, SIZE_MAX, 0};
  
  // intercept request message
  if (!pb_decode_delimited(&istream, Request_fields, &request)) {
    ERROR("Message decoding failed."CR, response.error_msg);
    return;
  }
  
  DEBUG("<- {reqid=%i}"CR, request.reqid);
  
  //set timestamp
  response.timestamp = micros();

  do {
    if (!SensorHandler::handleRequest(&request, &response, &istream, &ostream)) {
        ERROR(response.error_msg);
        if (!pb_encode_delimited(&ostream, Response_fields, &response)) {
            ERROR("Encoding of response message failed.");
            return;
        }
    }
  } while (request.stream && (!_serial->available()));  
}

bool StreamWrapper::write_callback(pb_ostream_t *stream, const uint8_t *buf, size_t count) {
    return _serial->write(buf, count) == count;
}

bool StreamWrapper::read_callback(pb_istream_t *stream, uint8_t *buf, size_t count) {
  size_t avail = 0;
  while (avail < count) {
    avail = _serial->available();
    delayMicroseconds(1);
  }
  size_t result = _serial->readBytes((char*)buf, constrain(avail,0,count));
  return result == count;
}

