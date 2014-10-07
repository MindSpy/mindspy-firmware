#include "Device.hpp"

Device::Device() {
    SetupBluetooth();
    SetupUsb();
}

Device::~Device() {
    // Dealocate sensors
    for(unsigned int i = 0; i < sensor.size(); i++) {
         delete sensor[i];
    }

    delete detector;
    delete protocol;
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
    // Reserve memory for 10 sensors.
    sensor.reserve(5);
    // Add sensors
    sensor.push_back(new TestSensor("TestSensor", 128, 8));
    sensor.push_back(new ADS1x9y());

    // Add sensor to detector
    detector = new SensorDetector(sensor);
    protocol = new ProtocolWrapper(detector);

    ProtocolWrapper::setStream(&PB_STREAM);
}
