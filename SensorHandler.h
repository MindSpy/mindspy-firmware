
#include "regs_pb.h"
#include "Sensor.h"
#include "macros.h"

#ifndef SensorHandler_h
#define SensorHandler_h

typedef bool (*ResponseEncoderCallbackType) (const pb_field_t[], const Response *);
typedef uint64_t (*TimestampCallbackType) (void);

/*! 
* Static class for handling the request, reading data from sensors and updating the responses.
*/
class SensorHandler {
    private:
        static ISensor** _sensors;
        static size_t _sensorCount;
        static ResponseEncoderCallbackType _encoder;
        static TimestampCallbackType _timestamp;

    public:
        /*!
        * Set initialize internal fields
        * \param sensors - array of pointers to sensors
        * \param responseEncoder - response encoder callback
        * \param timestampCallback - platform non-specific timestamp callback
        */
        template <size_t N>
        static void init(const ISensor* (& sensors) [N], ResponseEncoderCallbackType, TimestampCallbackType);

        /*!
        * Handle request and update response object
        * \param request structure
        * \param response structure
        * \param response encoder callback
        * \param platform non-specific timestamp callback
        */
        static bool handleRequest(Request*, Response*);
};

#endif
