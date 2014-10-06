#include "Device.hpp"

Device::Device() : sensors(NULL), detector(NULL), protocolWrapper(NULL) {
    SetupBluetooth();
    SetupUsb();
}

Device::~Device() {
    for (unsigned int i = 0; i < SENSOR_COUNT; i++) {
        delete sensors[i];
    }
    delete[] sensors;
    delete detector;
    delete protocolWrapper;
}

void Device::SetupBluetooth() {
    // Initialize pins for bluetooth module
    pinMode(GREEN_LED, INPUT);
    pinMode(BLUE_LED, INPUT);
    pinMode(PB_2, OUTPUT);
    pinMode(PB_3, OUTPUT);

    // Reset bluetooth.
    digitalWrite(PB_2, LOW);
    delay(20);
    digitalWrite(PB_2, HIGH);

    // Setup bluetooth.
    digitalWrite(PB_3, HIGH);
    // Initialize speed
    BLUETOOTH_STREAM.begin(BLUETOOTH_STREAM_INIT_BAUD);
    // Set new speed
    BLUETOOTH_STREAM.print("AT+UART=");
    BLUETOOTH_STREAM.print(BLUETOOTH_STREAM_BAUD);
    BLUETOOTH_STREAM.print(",1,0\r\n");
    BLUETOOTH_STREAM.print("AT+NAME=MindSpy\r\n");
    BLUETOOTH_STREAM.print("AT+ROLE=0\r\n");

    delay(10);
    digitalWrite(PB_3, LOW);

    // Start bt stream on a new speed.
    BLUETOOTH_STREAM.end();
    BLUETOOTH_STREAM.begin(BLUETOOTH_STREAM_BAUD); // max. 1382400=1.3Mbps
}

void Device::SetupUsb() {
    USB_STREAM.begin(USB_STREAM_BAUD);
}

void Device::SetupDevice() {
    // TODO this will be in SensorDetector once it is finished
    sensors = new sensor::Sensor*[SENSOR_COUNT];
    sensors[0] = new TestSensor("TestSensor", 128, 8);
    sensors[1] = new ADS1x9y(0);
    detector = new SensorDetector(sensors, SENSOR_COUNT);
    protocolWrapper = new ProtocolWrapper(detector);
    ProtocolWrapper::setStream(&PB_STREAM);
}
