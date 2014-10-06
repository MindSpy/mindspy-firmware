#ifndef PROTOCOLWRAPPER_H
#define PROTOCOLWRAPPER_H

#include "SensorHandler.hpp"

#ifndef _UNITTEST
#include <Arduino.h>
#else
#include "compat.hpp"
#endif

#ifndef _UNITTEST

#include <Arduino.h>

#else

#include <unistd.h>
// delayMicroseconds for unit testing (wiring is not available)
#define delayMicroseconds(us) usleep(us)
// Stream interface to be implemented by unit test (wiring not available)

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
    static bool write_callback(pb_ostream_t *, const uint8_t *, size_t);

    /*!
     * \brief Callback for nanopb input.
     */
    static bool read_callback(pb_istream_t *, uint8_t *, size_t);

    /*!
     * \brief Callback for SensorHandler::handle stop condition.
     * \return stop streaming
     */
    static bool stopStream(void);
};

#endif // PROTOCOLWRAPPER_H
