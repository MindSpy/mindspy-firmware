#include "Main.h"

#include <Arduino.h>
#include <SPI.h>

#include <pb.h>
#include <pb_encode.h>
#include <pb_decode.h>

#include "SensorHandler.h"
#include "ADS1x9y.h"
#include "macros.h"

#define LOGLEVEL LOG_LEVEL_VERBOSE
#include "Logging.h"

ISensor* sensors[] = { &Sensor };
SensorHandler sh  = SensorHandler(&pb_decode_delimited, &pb_encode_delimited, &timestamp, &stopStream, sensors, COUNT_OF(sensors));

uint64_t bootTime = 0;

bool stopStream(void) {
    return !!PB_STREAM.available();
}

uint64_t timestamp(void) {
    return bootTime + micros();
}

bool write_callback(pb_ostream_t *stream, const uint8_t *buf, size_t count) {
    Stream* s = (Stream*)stream->state;
    return s->write(buf, count) == count;
}

bool read_callback(pb_istream_t *stream, uint8_t *buf, size_t count) {
    Stream* s = (Stream*)stream->state;
    size_t avail = 0;
    // wait for enough data
    while (avail < count) {
        avail = s->available();
        delayMicroseconds(1);
    }
    size_t result = s->readBytes((char*)buf, constrain(avail,0,count));
    return result == count;
}

void handle(void) {
    // streams for nanopb
    pb_istream_t istream = {&read_callback, &PB_STREAM, SIZE_MAX};
    pb_ostream_t ostream = {&write_callback, &PB_STREAM, SIZE_MAX, 0};

    // request and response structures
    Request request;
    Response response;

    // response defaults
    response.timestamp = timestamp();
    response.has_module = false;
    response.states_count = 0;
    response.samples_count = 0;
    response.has_modelName = false;
    response.has_error_msg = false;

    // handle incomming request
    if (!sh.handle(&request, &response, &istream, &ostream)) {
        ERROR(response.error_msg);
        ERROR(CR);
        if (!pb_encode_delimited(&ostream, Response_fields, &response)) {
            ERROR("Encoding of response message failed."CR);
            return;
        }
    }
}

void setup() {
    // initialize pins for BT module
    pinMode(GREEN_LED, INPUT);
    pinMode(BLUE_LED, INPUT);
    pinMode(PB_2, OUTPUT);
    pinMode(PB_3, OUTPUT);

    // intialize logger
    LOG_STREAM.begin(LOG_STREAM_BAUD);
    Log.init(LOGLEVEL, &LOG_STREAM);

    // reset BT
    digitalWrite(PB_2, LOW);
    delay(20);
    digitalWrite(PB_2, HIGH);

    // setup BT
    digitalWrite(PB_3, HIGH);
    PB_STREAM.begin(BT_STREAM_INIT_BAUD); // intial speed
    PB_STREAM.print("AT+UART=" STRINGIFY(BT_STREAM_BAUD) ",1,0\r\n"); // set new speed
    PB_STREAM.print("AT+NAME=MindSpy\r\n");
    PB_STREAM.print("AT+ROLE=0\r\n");

    delay(10);
    digitalWrite(PB_3, LOW);

    // start bt stream on a new speed
    PB_STREAM.end();
    PB_STREAM.begin(BT_STREAM_BAUD); // max. 1382400=1.3Mbps

    // TODO: module autodetection
    // initialize sensor(s)
    Sensor.begin();
    Sensor.START(); // start A/D conversion
}

void loop() {
    // TODO: module autodetection, timestamp correction
    // process protocol buffers handler
    handle();
}