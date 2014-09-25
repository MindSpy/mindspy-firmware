#ifndef SensorHandler_h
#define SensorHandler_h

#include <stdio.h>
#include "Sensor.h"
#include "SensorDetector.h"

typedef bool (*ResponseEncoderCallbackType)(pb_ostream_t*, const pb_field_t[], const void *);
typedef bool (*RequestDecoderCallbackType)(pb_istream_t*, const pb_field_t[], void *);
typedef bool (*StopStreamCallbackType)(void);

namespace sensor {

/*!
 * \brief Static class for handling the request, reading data from sensors and updating the responses.
 */
class SensorHandler {
public:

    /*!
     * \briefConstructor call handling the request and reading data.
     * \param timestamp - callback for timestamp generation
     * \param stopStream - callback for stop condition when in stream mode
     * \param sensors - sensor detector instance
     */
    SensorHandler(StopStreamCallbackType, SensorDetector*);

    ~SensorHandler();

    /*!
     * \brief handle the request, contact the sensors and submit response.
     * \param input stream
     * \param output stream
     * \return true if success
     */
    bool handle(pb_istream_t*, pb_ostream_t*);

private:

    /*!
     * \brief stopStream
     */
    StopStreamCallbackType stopStream;

    /*!
     * \brief sensors
     */
    SensorDetector* sensors;

    /*!
     * \brief Request and response structures.
     */
    Request request;
    Response response;
};

} // namespace

#endif
