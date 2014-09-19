#include <pb_encode.h>
#include <pb_decode.h>
#include "pb_common.h"
#include "macros.h"
#include <Logging.h>



#define C_ARRAY_LENGTH(_name) sizeof(_name)/sizeof(*_name)

Stream*  StreamWrapper::_serial;
ISensor** StreamWrapper::_sensors;
uint8_t StreamWrapper::_sensor_count;

void StreamWrapper::init(Stream* serial, ISensor** sensors, uint8_t sensor_count) {
  _serial = serial;
  _sensors = sensors;
  _sensor_count = sensor_count;
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
  
  //defaults
  response.has_error_msg = false;
  response.timestamp = micros();
  response.reqid = request.reqid;
  
  ISensor* sensor = NULL;
  uint8_t module = 0;

  if (request.has_module) {
    module = request.module;
  }
  
  do {
    do {
      sensor = _sensors[module];    
      response.timestamp = micros();
      response.has_error_msg = false;
      response.module = module;
      
      if (request.has_setState) {
        if (!sensor->setState(request.setState.states, request.setState.states_count, NULL)) {
          ERROR("Method setState of module #%d failed."CR, module);
          return;
        }
      }
      
      if (request.has_getState) {
        if (!sensor->getState(request.getState.addresses, request.getState.addresses_count, response.states)) {
          ERROR("Method getState of module #%d failed."CR, module);
          return;
        }
        response.states_count = request.getState.addresses_count;
      }
      
      if (request.has_getSamples) {
        if (!sensor->getSamples(request.getSamples.count, response.samples)) {
          ERROR("Method getSamples of module #%d failed."CR, module);
          return;
        }
        response.samples_count = request.getSamples.count;
      }

      if (request.has_getModelName) {
        if (!sensor->getModelName(response.modelName)) {
          ERROR("Method getModelName of module #%d failed."CR, module);
          return;
        }
        response.has_modelName = true;
      }
      
      // serialize response message
      if (!pb_encode_delimited(&ostream, Response_fields, &response)) {
        ERROR("Serialization of response message failed."CR);
        return;
      }

      DEBUG("-> {reqid=%d}"CR, response.reqid);

    } while (!request.has_module && (++module < _sensor_count));
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

