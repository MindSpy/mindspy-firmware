#ifndef SENSORHANDLER_HPP
#define SENSORHANDLER_HPP

#include <stdio.h>
#include "Sensors.hpp"

#define DECLARED_ARRAY_ITEM_COUNT(x) (sizeof(x)/sizeof(x[0]))

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
        SensorHandler();

        ~SensorHandler();

        /*!
         * \brief handle the request, contact the sensors and submit response.
         * \param input stream
         * \param output stream
         * \return true if success
         */
        bool handle(pb_istream_t*, pb_ostream_t*);

        /*!
         * \brief setSensors
         * \param sensors instance to set
         */
        void setSensors(Sensors*);

        /*!
         * \brief setStopCallback
         * \param stop stream callback to set
         */
        void setStopCallback(StopStreamCallbackType);

    private:

        /*!
         * \brief stopStream
         */
        StopStreamCallbackType stopStream;

        /*!
         * \brief sensors
         */
        Sensors* sensors;

        /*!
         * \brief Request and response structures.
         */
        Request request;
        Response response;

        bool respond(Sensor*, Request, Response);
    };

} // namespace

#endif
