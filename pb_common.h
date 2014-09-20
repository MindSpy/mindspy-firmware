#ifndef PB_COMMONS_H
#define PB_COMMONS_H

#include <Arduino.h>
#include <pb.h>
#include "regs_pb.h"
#include "Sensor.h"

/*!
 * IO stream handlers wraper.
 * Class for an request message interception, handler invocation and response genereation.
 */
class StreamWrapper {
  protected:
    static Stream* _serial;
    static ISensor** _sensors;
    static uint8_t _sensor_count;

    static bool write_callback(pb_ostream_t *, const uint8_t *, size_t );
    static bool read_callback(pb_istream_t *, uint8_t*, size_t );
  public:

    /*!
     * Initializes StreamWrapper with stream instance and response handlers.
     * \param serial stream instance
     * \param sensors array of sensor modules
     * \param sensor_count sensor module count
     * \return void
     */
    static void init(Stream* stream, ISensor** sensors, size_t sensorCount);

    /*!
     * Receives request message and returns response. Intercepts message
     * from the input stream, performs an action based on the request then
     * generates and submits the response message to the output stream.
     * \param void
     * \return void
     */
    static void handle(void);

    /*!
     * Not implemented.
     */
    static void error(Response*,const char[]);
};



#endif
