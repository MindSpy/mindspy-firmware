#include <pb_encode.h>
#include <pb_decode.h>
#include "pb_common.h"

// TODO: this is in question - does it work? 
#ifndef SIZE_MAX
#define SIZE_MAX ~((size_t)0)
#endif

#define C_ARRAY_LENGTH(_name) sizeof(_name)/sizeof(*_name)

Stream*  StreamWrapper::_serial;
response_handler_t* StreamWrapper::_handlers;
uint8_t StreamWrapper::_handler_count;
pb_istream_t StreamWrapper::_input = {&StreamWrapper::read_callback, NULL, SIZE_MAX};
pb_ostream_t StreamWrapper::_output = {&StreamWrapper::write_callback, NULL, SIZE_MAX, 0};

void StreamWrapper::init(Stream* serial, response_handler_t* handlers, uint8_t handler_count) {
  _serial = serial;
  _handlers = handlers;
  _handler_count = handler_count;
}

void StreamWrapper::handle(void) {
  Request request;
  Response response;
  
  // intercept request message
  if (!pb_decode(input(), Request_fields, &request)) {
    return;
  }
  
  //defaults
  response.has_error = false;
  response.has_error_msg = false;
  response.timestamp = micros();
  response.sample.funcs.encode = NULL;
  response.state.funcs.encode = NULL;
  
  // determine handler based on request
  bool has_handler = false;    
  for (int i = 0; i < _handler_count; i++) {
    if (_handlers[i].action == request.action) {
      if (!(*_handlers[i].handler)(&request, &response)) {
        if (!response.has_error){
          response.has_error = true;
          response.error = Response_ErrNo_HANDLER_FAILED;
          if (!response.has_error_msg) {
            response.has_error_msg = true;
            sprintf(response.error_msg, "No error message provided.");
          }
        }
      }
      has_handler = true;
      break;
    }
  }
  if (!has_handler) {
    response.has_error = true;
    response.error = Response_ErrNo_HANDLER_MISSING;
    response.has_error_msg = true;
    sprintf(response.error_msg, "Missing handler for action #.");
     // this will add another 18kB of source code
    //sprintf(response.error_msg, "Missing handler for action #%d.", request.action);
  }
  
  // serialize response message
  if (!pb_encode(output(), Response_fields, &response)) {
    return;
  }
}

bool StreamWrapper::write_callback(pb_ostream_t *stream, const uint8_t *buf, size_t count) {
    return _serial->write(buf, count) == count;
}

bool StreamWrapper::read_callback(pb_istream_t *stream, uint8_t *buf, size_t count) {
  uint32_t avail = 0;
  while(!(avail = _serial->available())) delayMicroseconds(2);
  uint32_t result = _serial->readBytes((char*)buf, constrain(avail,0,count));
  stream->bytes_left -= result;
  return result == count;
}

