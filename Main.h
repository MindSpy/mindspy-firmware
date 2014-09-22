
#ifndef Main_h
#define Main_h

#include <Arduino.h>
#include <pb.h>

// max. 1382400=1.3Mbps
#define BT_STREAM_BAUD 921600
#define BT_STREAM_INIT_BAUD 38400

#define LOG_STREAM_BAUD 115200

#define PB_STREAM Serial1
#define LOG_STREAM Serial

/*!
 * executed on boot by wiring
 */
void setup(void);

/*!
 * executed within infinity loop by wiring
 */
void loop(void);

/*!
 * callback for nanopb output
 */
bool write_callback(pb_ostream_t *, const uint8_t *, size_t );

/*!
 * callback for nanopb input
 */
bool read_callback(pb_istream_t *, uint8_t*, size_t );

/*!
 * callback for SensorHandler::handle stop condition
 * \return determine when to stop streaming
 */
bool stopStream(void);

/*!
 * callback for SensorHandler::handle timestamp generator
 * \return time in microseconds from reference
 */
uint64_t timestamp(void);

/*!
 * callback for nanopb input
 */
void handle(void);

#endif