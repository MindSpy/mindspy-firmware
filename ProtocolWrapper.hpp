#ifndef PROTOCOLWRAPPER_HPP
#define PROTOCOLWRAPPER_HPP

#include "SensorHandler.hpp"
#include "Sensors.hpp"

#ifndef _UNITTEST
#include <Arduino.h>
#else
#include "compat.hpp"
#endif

/*!
 * \brief The Main class
 */
class ProtocolWrapper {
public:
    /*!
     * \brief ProtocolBuffersWrapper
     */
    ProtocolWrapper();      
    ~ProtocolWrapper();
    /*!
     * \brief handle
     * \return
     */
    bool handle();

    /*!
     * \brief setStream
     * \param stream to set
     */
    static void setStream(Stream*);

    /*!
     * \brief setSensors
     * \param sensors collection to set
     */
    void setSensors(Sensors*);

private:
    sensor::SensorHandler handler;

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
