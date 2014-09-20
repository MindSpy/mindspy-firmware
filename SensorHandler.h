
#include "regs_pb.h"
#include "Sensor.h"

#ifndef SensorHandler_h
#define SensorHandler_h

typedef bool (*ResponseEncoderCallbackType) (pb_ostream_t*, const pb_field_t[], const void *);
typedef bool (*RequestDecoderCallbackType) (pb_istream_t*, const pb_field_t[], void *);
typedef uint32_t (*TimestampCallbackType) (void);
typedef bool (*StopStreamCallbackType) (void);

/*! 
* Static class for handling the request, reading data from sensors and updating the responses.
*/
class SensorHandler {
    private:
        static ISensor** _sensors;
        static size_t _sensorCount;
        static ResponseEncoderCallbackType _encoder;
        static RequestDecoderCallbackType _decoder;
        static TimestampCallbackType _timestamp;
        static StopStreamCallbackType _stop;

    public:
        /*!
        * Set initialize internal fields
        * \param sensors - array of pointers to sensors
        * \param responseEncoder - response encoder callback
        * \param timestampCallback - platform non-specific timestamp callback
        */
        static void init(RequestDecoderCallbackType, ResponseEncoderCallbackType, TimestampCallbackType, StopStreamCallbackType);

        /*!
         *
         */
        static void init(ISensor**, size_t);

        /*!
        * Handle request and update response object
        * \param request structure
        * \param response structure
        * \param response encoder callback
        * \param platform non-specific timestamp callback
        */
        static bool handle(Request*, Response*, pb_istream_t*, pb_ostream_t*);
};

#endif
