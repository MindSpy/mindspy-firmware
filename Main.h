#ifndef Main_h
#define Main_h

#include <Arduino.h>
#include <pb.h>

// max. for HC05 is 1382400=1.3Mbps
const int BLUETOOTH_STREAM_BAUD = 921600;
const int BLUETOOTH_STREAM_INIT_BAUD = 38400;
const int USB_STREAM_BAUD = 115200;
// this shoud be macro ..
#define BLUETOOTH_STREAM Serial1
#define USB_STREAM Serial
// choose one stream for protocol buffers
#define PB_STREAM BLUETOOTH_STREAM

/*!
 * \brief Executed on boot by wiring.
 */
void setup(void);

/*!
 * \brief setupBT
 */
void setupBT(void);

/*!
 * \brief setupUsb
 */
void setupUsb(void);

/*!
 * \brief Executed within infinity loop by wiring
 */
void loop(void);

/*!
 * \brief Callback for nanopb output.
 */
bool write_callback(pb_ostream_t *, const uint8_t *, size_t);

/*!
 * \brief Callback for nanopb input.
 */
bool read_callback(pb_istream_t *, uint8_t*, size_t);

/*!
 * \brief Callback for SensorHandler::handle stop condition.
 * \return stop streaming
 */
bool stopStream(void);

/*!
 * \brief Callback for SensorHandler::handle timestamp generator
 * \return elapsed time from reference in microseconds
 */
uint64_t timestamp(void);

#endif
