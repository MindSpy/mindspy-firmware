#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "ADS1x9y.hpp"
#include "ProtocolWrapper.hpp"
#include "SensorDetector.hpp"
#include "Sensors.hpp"

/*!
 * Stream data from bluetooth device.
 */
#define BLUETOOTH_STREAM Serial1

/*!
 * Stream data from serial interface.
 */
#define USB_STREAM Serial

/*!
 * Choose one stream for protocol buffers.
 */
#define PB_STREAM USB_STREAM

/*!
 * Setting and inicialize all device and peripheral.
 */
class Device {
public:

    /*!
     * Prepare all device and peripherals.
     */
    Device();

    /*!
     * Destroy all create objects and instances.
     */
    ~Device();

    /*!
     * \brief setup the device
     */
    void setup();

    /*!
     * Prepare all sensors.
     */
    void setupDevice();

    /*!
     * Prepare USB device.
     */
    void setupUsb();

    /*!
     * Prepare Bluetooth device.
     */
    void setupBluetooth();

    SensorDetector sensorDetector;
    ProtocolWrapper protocolWrapper;

private:

    /*!
     * Tranfer speed for Bluetooth.
     * Maximum speed is for HC05 is 1382400=1.3Mbps
     */
    const static int BLUETOOTH_STREAM_BAUD = 921600;

    /*!
     * Init transfer speed fot Bluetooth.
     */
    const static int BLUETOOTH_STREAM_INIT_BAUD = 38400;

    /*!
     * Transfer speed for USB device.
     */
    const static int USB_STREAM_BAUD = 115200;
};
#endif // DEVICE_H
