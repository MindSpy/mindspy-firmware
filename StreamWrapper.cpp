#define LOGLEVEL LOG_LEVEL_VERBOSE

#include <pb_encode.h>
#include <pb_decode.h>
#include <Logging.h>
#include "StreamWrapper.h"
#include "SensorHandler.h"
#include "macros.h"

Stream*  StreamWrapper::_serial;

bool StreamWrapper::stopStream(void) {
    return !!_serial->available();
}

void StreamWrapper::init(Stream* serial) {
    _serial = serial;
    SensorHandler::init(&pb_decode_delimited, &pb_encode_delimited, &micros, &StreamWrapper::stopStream);
}

void StreamWrapper::handle(void) {
    Request request;
    Response response;

    pb_istream_t istream = {&StreamWrapper::read_callback, StreamWrapper::_serial, SIZE_MAX};
    pb_ostream_t ostream = {&StreamWrapper::write_callback, StreamWrapper::_serial, SIZE_MAX, 0};

    //set timestamp
    response.timestamp = micros();

    if (!SensorHandler::handle(&request, &response, &istream, &ostream)) {
        ERROR(response.error_msg);
        if (!pb_encode_delimited(&ostream, Response_fields, &response)) {
            ERROR("Encoding of response message failed.");
            return;
        }
    }
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

