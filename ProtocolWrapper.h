#ifndef PROTOCOLWRAPPER_H
#define PROTOCOLWRAPPER_H


#include "SensorHandler.h"

#ifndef _UNITTEST

#include <Arduino.h>

#else

#include <unistd.h>
// interface to be implemented by unit test
class Stream {
    public:
        virtual ~Stream() {}
        virtual int available() = 0;
        virtual size_t readBytes( char *buffer, size_t length) = 0;
        virtual size_t write(const uint8_t *buffer, size_t size) = 0;
};

#define delayMicroseconds(us) usleep(us)

#endif


/*!
 * \brief The Main class
 */
class ProtocolWrapper {
    public:
        /*!
         * \brief ProtocolBuffersWrapper
         */
        ProtocolWrapper(SensorDetector*);
        /*!
         * \brief handle
         * \return
         */
        bool handle();

        static void setStream(Stream*);

    private:
        sensor::SensorHandler* handler;

        static Stream* stream;

        /*!
         * \brief Callback for nanopb input.
         */
        static bool write_callback(pb_ostream_t *, const uint8_t *, size_t );

        /*!
         * \brief Callback for nanopb input.
         */
        static bool read_callback(pb_istream_t *, uint8_t *, size_t );

        /*!
         * \brief Callback for SensorHandler::handle stop condition.
         * \return stop streaming
         */
        static bool stopStream(void);

        /*!
         * \brief Callback for SensorHandler::handle timestamp generator
         * \return elapsed time from reference in microseconds
         */
        static uint64_t timestamp(void);
};

#endif // PROTOCOLWRAPPER_H
