

#include <Arduino.h>
#include "SPI.h"
#include "Logging.h"
#include "ADS1298.h"

ADS1298  Sensor;

void setup() {
  Serial.begin(115200);
  Log.init(LOG_LEVEL_VERBOSE, &Serial);
  Sensor.begin();
  delay(1000);
  test();
}

void test() {
  //Sensor.setRegister(0x01, 0x11);
  Log.debug("Registers:"CR);
  for (int i = 0; i < 5; i++) {
    byte r = Sensor.getRegister(i);
    Log.debug("%X -> %d"CR, i, r);
  }
}

void loop() {
  delay(1000);
}

