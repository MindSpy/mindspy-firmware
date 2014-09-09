
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
  //response->module = Sensor.getModule();
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
  //response->module = Sensor.getModule();
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
  //response->module = Sensor.getModule(); 
  return true;
}

// define callbacks for patricular data structures types
bool get_state_callback(pb_ostream_t *stream, const pb_field_t *field, void * const *arg) {

  Request* req = (Request*)*arg;
  State state;
  uint8_t* buffer = (uint8_t*)malloc(req->count*sizeof(uint8_t));
  bool err = false;
  
  Sensor.RREG(req->start, req->count, buffer);
  
  for (uint32_t i = 0, a = req->start; i < req->count; i++, a++) {
    state.address = a;
    state.payload = buffer[i];
    if (!pb_encode_tag_for_field(stream, field))
      err = true;
    if (!pb_encode_submessage(stream, State_fields, &state))
      err = true;
  }
  free(buffer);
  return !err;
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
  bool err = false;
  
  for (uint32_t i = 0; i < req->count; i++) {
    
    // wait for sample
    while(!Sensor);
    Sensor.RDATA(buffer, bsize);
    // unpack the buffer
    size_t idx = 3; // skip 3 bytes - status registers
    for (int j = 0; j < channels; j++) {
      int32_t tmp = 0;
      for (int k = 0; k < bytes; k++) {
        tmp<<=8;
        tmp|=buffer[idx++];
      }
      samp.payload[j] = tmp;
    }
    samp.sequence = sequence++;
    samp.payload_count = channels;
    
    if (!pb_encode_tag_for_field(stream, field)) {
      err = true;
      break;
    }
    if (!pb_encode_submessage(stream, Sample_fields, &samp)){
      err = true;
      break;
    }
  }
  free(buffer);
  Sensor.SDATAC();
  return !err;
}


void setup() {
  
  pinMode(GREEN_LED, INPUT);
  pinMode(BLUE_LED, INPUT);
  pinMode(PB_2, OUTPUT);
  pinMode(PB_3, OUTPUT);
  
  Serial.begin(115200);
  Log.init(LOG_LEVEL_VERBOSE, &Serial);
  
  // reset BT
  digitalWrite(PB_2, LOW);
  delay(20);
  digitalWrite(PB_2, HIGH);
  
  // setup BT
  digitalWrite(PB_3, HIGH);
  Serial1.begin(38400);
  Serial1.print("AT+UART=921600,1,0\r\n"); // max. 1382400=1.3Mbps
  Serial1.print("AT+NAME=MindSpy\r\n");
  Serial1.print("AT+ROLE=0\r\n");
  delay(10);
  digitalWrite(PB_3, LOW);
    
  Serial1.end();
  Serial1.begin(921600); // max. 1382400=1.3Mbps
  StreamWrapper::init(&Serial1, handlers, C_ARRAY_LENGTH(handlers));
    
  Sensor.begin();
  Sensor.START();
}

void loop() {
  StreamWrapper::handle();
}

