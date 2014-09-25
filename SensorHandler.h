#ifndef SensorHandler_h
#define SensorHandler_h

#include <stdio.h>
#include "Sensor.h"

typedef bool (*ResponseEncoderCallbackType)(pb_ostream_t*, const pb_field_t[], const void *);
typedef bool (*RequestDecoderCallbackType)(pb_istream_t*, const pb_field_t[], void *);
typedef uint64_t (*TimestampCallbackType)(void);
typedef bool (*StopStreamCallbackType)(void);

namespace sensor {

/**
 * Static class for handling the request, reading data from sensors and updating the responses.
 */
class SensorHandler {
public:

    /**
     * Constructor call handling the request and reading data.
     * \param decoder - callback for request decoding
     * \param encoder - callback for response encodding
     * \param timestamp - callback for timestamp generation
     * \param stop - callback for stop condition when in stream mode
     * \param sensors - array of pointers to sensors
     * \param responseEncoder - response encoder callback
     */
    SensorHandler(TimestampCallbackType time, StopStreamCallbackType stop, sensor::Sensor** sensor, size_t sensors);
    ~SensorHandler();

    /**
     * Handle request and update response object.
     * \param response encoder callback
     * \param platform non-specific timestamp callback
     */
    bool handle(pb_istream_t*, pb_ostream_t*);

private:

    /**
     *
     */
    TimestampCallbackType timesStamp;

    /**
     *
     */
    StopStreamCallbackType stopStream;

    /**
     *
     */
    sensor::Sensor** sensors;

    /**
     *
     */
    sensor::Sensor* boardSensor;

    /**
     * Count of senzors for board.
     */
    size_t countOfSensors;

    /**
     * Request and response structures.
     */
    Request request;
    Response response;
};

} // namespace

#endif
