#define LOGLEVEL LOG_LEVEL_VERBOSE


#include <Arduino.h>
#include <SPI.h>
#include <Logging.h>
#include "ADS1x9y.h"
#include "regs_pb.h"
#include "pb_common.h"
#include "macros.h"

#define C_ARRAY_LENGTH(_name) sizeof(_name)/sizeof(*_name)


ISensor* sensors[] = { &Sensor };

void setup() {

  pinMode(GREEN_LED, INPUT);
  pinMode(BLUE_LED, INPUT);
  pinMode(PB_2, OUTPUT);
  pinMode(PB_3, OUTPUT);

  Serial.begin(115200);
#ifdef LOGLEVEL
  Log.init(LOGLEVEL, &Serial);
#endif

  // reset BT
  digitalWrite(PB_2, LOW);
  delay(20);
  digitalWrite(PB_2, HIGH);

  // setup BT
  digitalWrite(PB_3, HIGH);
  Serial1.begin(38400);
  Serial1.print("AT+UART=921600,1,0\r\n"); // max. 1382400=1.3Mbps
  Serial1.print("AT+NAME=MindSpy\r\n");
  Serial1.print("AT+ROLE=0\r\n");
  delay(10);
  digitalWrite(PB_3, LOW);

  Serial1.end();
  Serial1.begin(921600); // max. 1382400=1.3Mbps

  StreamWrapper::init(&Serial1, sensors, COUNT_OF(sensors));

  Sensor.begin();
  Sensor.START();
}

void loop() {
  StreamWrapper::handle();
}

