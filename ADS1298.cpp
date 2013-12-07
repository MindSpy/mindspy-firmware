/*
ADS1298 Analog Frontend 

created 30 Nov 2013
by pBorky
*/


#include <Arduino.h>
#include "SPI.h"
#include "Logging.h"
#include "ADS1298.h"

// Sensor Pinout definition
// SPI pins
#define DRDY  PC_6
#define MOSI  PA_6
#define MISO  PA_5
#define SCLK  PA_2
#define CS1_  PA_3
#define CS2_  PF_4
#define CS3_  PE_1
#define CS4_  PB_4
#define CS5_  PD_3
  
// control pins
#define RESET_ PA_6
#define START  PA_7
#define PWDN_  PE_3

// I/O pins
#define GPIO1  PD_7
#define GPIO2  PB_6
#define GPIO3  PC_7
#define GPIO4  PB_7

// sensor's internal clock
#define ICLK   PE_3

static const unsigned int SPI_CS_PIN_COUNT = 5;
static const unsigned int SPI_CS_PINS[SPI_CS_PIN_COUNT] = {
    CS1_, CS2_, CS3_, CS4_, CS5_
};
static const unsigned int GPIO_PIN_COUNT = 4;
static const unsigned int GPIO_PINS[GPIO_PIN_COUNT] = {
  GPIO1, GPIO2, GPIO3, GPIO4
};

#define SPI_CS(module) SPI_CS_PINS[module]
#define GPIO(pin) GPIO_PINS[pin]

boolean ADS1298::pinSet = true;
SPIClass ADS1298::spi(0);

ADS1298::ADS1298() {
  _module = 0;
  _cs = SPI_CS(_module);
}
ADS1298::ADS1298(int module) {
  if (module < 0) {
    _module = 0;
  } else if (module >= SPI_CS_PIN_COUNT) {
    _module = SPI_CS_PIN_COUNT-1;
  } else {
    _module = module;
  }
  _cs = SPI_CS(_module);
}

void ADS1298::begin() {
  Log.debug("Begin on cs=%d."CR, _cs);
  pinSetup();
  spi.begin(_cs);
  spi.setClockDivider(SPI_CLOCK_DIV32);
  spi.setDataMode(SPI_MODE1);
}

void ADS1298::end() {
  Log.debug("End on cs=%d."CR, _cs);
  spi.end(_cs);
}

byte ADS1298::getRegister(int reg) {
  byte op = (reg&0x1f)|0x20;
  Log.verbose("opcodes: %X %X"CR, op, 0);
  byte r = spi.transfer(_cs, op, SPI_CONTINUE);
  r = spi.transfer(_cs, 0x01, SPI_CONTINUE);
  r = spi.transfer(_cs, 0x00, SPI_CONTINUE);
  r = spi.transfer(_cs, 0x00);
  return r;
}

void ADS1298::setRegister(int reg, byte data) {
  byte op = (reg&0x1f)|0x40;
  Log.verbose("opcode: %B %B %B"CR, op, 0, data);
  byte r = spi.transfer(_cs, op, SPI_CONTINUE);
  r = spi.transfer(_cs, 0x00, SPI_CONTINUE);
  r = spi.transfer(_cs, data, SPI_CONTINUE);
  r = spi.transfer(_cs, 0x00);
}

void ADS1298::start() {
  digitalWrite(START, HIGH);
}

void ADS1298::stop() {
  digitalWrite(START, LOW);
}

void ADS1298::reset() {
  digitalWrite(RESET_, LOW);
  delay(10);
  digitalWrite(RESET_, HIGH);
}

void ADS1298::pwrdn() {
  digitalWrite(PWDN_, LOW);
}

void ADS1298::pwrup() {
  digitalWrite(PWDN_, HIGH);
}

void ADS1298::gpioMode(int pin, int mode) {
  digitalWrite(GPIO(pin), mode);
}

int ADS1298::gpioRead(int pin) {
  return digitalRead(GPIO(pin));
}

void ADS1298::gpioWrite(int pin, int value) {
  digitalWrite(GPIO(pin), value);
}

void ADS1298::pinSetup() {
  pinMode(_cs,  OUTPUT);
  digitalWrite(_cs, HIGH);
  
  if (pinSet) {
    pinMode(DRDY, INPUT);
    pinMode(MOSI, OUTPUT);
    pinMode(MISO, INPUT);
    pinMode(SCLK, OUTPUT);
    
    pinMode(RESET_, OUTPUT);
    pinMode(START, OUTPUT);
    pinMode(PWDN_, OUTPUT);
    
    digitalWrite(RESET_, HIGH);
    digitalWrite(START, LOW);
    digitalWrite(PWDN_, HIGH);
    
    pinMode(GPIO1, INPUT);
    pinMode(GPIO2, INPUT);
    pinMode(GPIO3, INPUT);
    pinMode(GPIO4, INPUT);
    
    pinMode(ICLK, INPUT);
    
    pinSet = false;
  }
}
