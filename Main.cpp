
#include "Main.h"
#include "SensorDetector.h"
#include "ADS1x9y.h"
#include "TestSensor.h"
#include "ProtocolWrapper.h"
#include "macros.h"

ProtocolWrapper* wrapper = NULL;

void setupBluetooth() {
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

void setupUsb() {
    USB_STREAM.begin(USB_STREAM_BAUD);
}

void setup() {
    // initialize BT
    setupBluetooth();
    // Initialize USB
    setupUsb();

    // TODO this will be in SensorDetector once it is finished
    const size_t SENSOR_COUNT = 2;
    sensor::Sensor** sensors = new sensor::Sensor*[SENSOR_COUNT];
    sensors[0] = new TestSensor("TestSensor", 128, 8);
    sensors[1] = new ADS1x9y(0);
    SensorDetector* detector = new SensorDetector(sensors, SENSOR_COUNT);
    // ~~

    wrapper = new ProtocolWrapper(detector);
    ProtocolWrapper::setStream(&PB_STREAM);
}

void loop() {
    if (wrapper != NULL)
    {
        wrapper->handle();
    }
}
