#ifndef SensorHandler_h
#define SensorHandler_h

#include "Sensor.h"

typedef bool (*ResponseEncoderCallbackType) (pb_ostream_t*, const pb_field_t[], const void *);
typedef bool (*RequestDecoderCallbackType) (pb_istream_t*, const pb_field_t[], void *);
typedef uint64_t (*TimestampCallbackType) (void);
typedef bool (*StopStreamCallbackType) (void);

/*! 
* Static class for handling the request, reading data from sensors and updating the responses.
*/
class SensorHandler {
    private:
        TimestampCallbackType _timestamp;
        StopStreamCallbackType _stop;
        ISensor** _sensors;
        size_t _sensorCount;

    public:
        /*!
        * Constructor
        * \param decoder - callback for request decoding
        * \param encoder - callback for response encodding
        * \param timestamp - callback for timestamp generation
        * \param stop - callback for stop condition when in stream mode
        * \param sensors - array of pointers to sensors
        * \param responseEncoder - response encoder callback
        */
        SensorHandler(TimestampCallbackType time, StopStreamCallbackType stop, ISensor** sense, size_t nsense) :
        _timestamp(time), _stop(stop), _sensors(sense), _sensorCount(nsense)
        {}

        /*!
        * Handle request and update response object
        * \param response encoder callback
        * \param platform non-specific timestamp callback
        */
        bool handle(pb_istream_t*, pb_ostream_t*);
};

#endif
