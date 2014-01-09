
#include <Arduino.h>
#include <SPI.h>
#include <Logging.h>
#include <pb.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include "ADS1x9y.h"
#include "pb_common.h"

#define C_ARRAY_LENGTH(_name) sizeof(_name)/sizeof(*_name)
  
// define handler - action mapping
response_handler_t handlers[] = {
  {Request_Action_SAMPLES, &samples_handler},
  {Request_Action_GET_STATE, &get_state_handler},
  {Request_Action_SET_STATE, &set_state_handler},
  {Request_Action_ECHO, &echo_handler}
};

// define handlers
bool echo_handler(Request* request, Response* response) {
  return true;
}

bool samples_handler(Request* request, Response* response) {
  if (!request->has_count) {
    response->has_error_msg = true;
    sprintf(response->error_msg, "missing `count`");
    return false;
  }
  response->sample.funcs.encode = &samples_callback;
  response->sample.arg = request;
  return true;
}

bool get_state_handler(Request* request, Response* response) {
  if (!request->has_count || !request->has_start) {
    response->has_error_msg = true;
    sprintf(response->error_msg, "missing `count` and/or `start`");
    return false;
  }
  response->state.funcs.encode = &get_state_callback;
  response->state.arg = request;
  return true;
}

bool set_state_handler(Request* request, Response* response) {
  if (!request->has_start || !request->has_payload || request->payload.size==0) {
    response->has_error_msg = true;
    sprintf(response->error_msg, "missing `payload` and/or `start`");
    return false;
  }
  response->state.funcs.encode = &set_state_callback;
  response->state.arg = request;
  return true;
}

// define callbacks for patricular data structures types
bool get_state_callback(pb_ostream_t *stream, const pb_field_t *field, void * const *arg) {

  Request* req = (Request*)*arg;
  State state;
  uint8_t* buffer = (uint8_t*)malloc(req->count*sizeof(uint8_t));
  Sensor.RREG(req->start, req->count, buffer);
  
  for (uint32_t i = 0, a = req->start; i < req->count; i++, a++) {
    state.address = a;
    state.payload = buffer[i];
    if (!pb_encode_tag_for_field(stream, field))
      return false;
    if (!pb_encode_submessage(stream, State_fields, &state))
      return false;
  }
  return true;
}

bool set_state_callback(pb_ostream_t *stream, const pb_field_t *field, void * const *arg) {
  Request* req = (Request*)*arg;
  Sensor.WREG(req->start, req->payload.size, req->payload.bytes);
  req->count = req->payload.size;
  return get_state_callback(stream, field, arg);
}

uint64_t sequence = 0;

bool samples_callback(pb_ostream_t *stream, const pb_field_t *field, void * const *arg) {

  Request* req = (Request*)*arg;
  Sample samp;
  
  uint8_t channels = Sensor.getChannelCount();
  uint8_t bytes = Sensor.getSampleBytes();
  uint16_t bsize = Sensor.getTotalBytes();
  uint8_t* buffer = (uint8_t*)malloc(bsize*sizeof(uint8_t));
  
  for (uint32_t i = 0; i < req->count; i++) {
    
    // wait for sample
    while(!Sensor);
    
    Sensor.RDATA(buffer, bsize);
    for (int j = 0; j < channels; j++) {
      int32_t tmp = 0;
      for (int k = 0; k < bytes; k++) {
        tmp<<=8;
        tmp|=buffer[3+j*bytes+k];// skip 3 bytes at begining - status registers
      }
      samp.payload[j] = tmp;
    }
    samp.sequence = sequence++;
    samp.payload_count = channels;
    
    if (!pb_encode_tag_for_field(stream, field))
      return false;
    if (!pb_encode_submessage(stream, Sample_fields, &samp))
      return false;
  }
  Sensor.SDATAC();
  return true;
}

void setup() {
  Serial.begin(921600);
  Serial1.begin(9600);
  Sensor.begin();
  Sensor.START();
  StreamWrapper::init(&Serial, handlers, C_ARRAY_LENGTH(handlers));
  Log.init(LOG_LEVEL_VERBOSE, &Serial1);
}

void loop() {
  StreamWrapper::handle();
}

