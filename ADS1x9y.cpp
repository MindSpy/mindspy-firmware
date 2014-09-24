/*
 ADS1x9y Analog Frontend

 created 30 Nov 2013
 by pBorky
 */

#include "ADS1x9y.h"

#include <SPI.h>
#include <Arduino.h>

#include "Sensor.h"

#define LOGLEVEL LOG_LEVEL_VERBOSE
#include "Logging.h"

// Sensor Pinout definition
// SPI pins
#define PIN_DRDY_  PC_6
#define PIN_MOSI  PA_5
#define PIN_MISO  PA_4
#define PIN_SCLK  PA_2
#define CS1_  PA_3
#define CS2_  PF_4
#define CS3_  PE_1
#define CS4_  PB_4
#define CS5_  PD_3

// control pins
#define PIN_RESET_ PA_6
#define PIN_START  PA_7
#define PIN_PWDN_  PE_2

// I/O pins
#define PIN_GPIO1  PD_7
#define PIN_GPIO2  PB_6
#define PIN_GPIO3  PC_7
#define PIN_GPIO4  PB_7

// sensor's internal clock
#define  PIN_ICLK  PE_3

#define SPI_CMD_WAKEUP  0x02
#define SPI_CMD_STANDBY 0x04
#define SPI_CMD_RESET   0x06
#define SPI_CMD_START   0x08
#define SPI_CMD_STOP    0x0A
#define SPI_CMD_RDATAC  0x10
#define SPI_CMD_SDATAC  0x11
#define SPI_CMD_RDATA   0x12
#define SPI_CMD_RREG    0x20
#define SPI_CMD_WREG    0x40

#define SPI_CS_PIN_COUNT 5
#define GPIO_PIN_COUNT 4

static const unsigned int SPI_CS_PINS[SPI_CS_PIN_COUNT] = {
CS1_, CS2_, CS3_, CS4_, CS5_ };
static const unsigned int GPIO_PINS[GPIO_PIN_COUNT] = {
PIN_GPIO1, PIN_GPIO2, PIN_GPIO3, PIN_GPIO4 };

#define SPI_CS(module) SPI_CS_PINS[module]
#define GPIO(pin) GPIO_PINS[pin]

boolean ADS1x9y::_pinSet = false;
int ADS1x9y::_active = -1;
int ADS1x9y::_sequence = 0;

ADS1x9y::ADS1x9y(uint8_t module = 0) {
	_module = constrain(module, 0, SPI_CS_PIN_COUNT-1);
	_continous = true;
	_channels = 0;
	_cs = SPI_CS(_module); // here: module = board
}

bool ADS1x9y::getSamples(uint32_t count, Sample* result) {

	uint8_t channels = this->getChannelCount();
	uint8_t bytes = this->getSampleBytes();
	uint16_t bsize = this->getTotalBytes();
	uint8_t* buffer = (uint8_t*) malloc(bsize * sizeof(uint8_t));

	for (uint32_t i = 0; i < count; i++) {
		Sample* samp = &(result[i]);

		// wait for sample
		while (!this)
			;
		RDATA(buffer, bsize);
		// unpack the buffer
		size_t idx = 3; // skip 3 bytes - status registers
		for (int j = 0; j < channels; j++) {
			int32_t tmp = 0;
			for (int k = 0; k < bytes; k++) {
				tmp <<= 8;
				tmp |= buffer[idx++];
			}
			samp->payload[j] = tmp;
		}
		samp->sequence = _sequence++;
		samp->payload_count = channels;

	}
	free(buffer);
	analogSensor.SDATAC();
	return true;
}

bool ADS1x9y::getState(uint32_t* addresses, uint32_t addresses_count,
		State* result) {
	uint8_t buffer = 0;

	for (uint32_t i = 0; i < addresses_count; i++) {
		this->RREG(addresses[i], 1, &buffer);

		result[i].address = addresses[i];
		result[i].payload = buffer;
	}

	return true;
}

bool ADS1x9y::setState(State* states, uint32_t states_count, void* result) {
	uint8_t p;
	for (uint32_t i = 0; i < states_count; i++) {
		p = states[i].payload;
		this->WREG(states[i].address, 1, &p);
	}
	return true;
}

bool ADS1x9y::getModelName(char* modelName) {
	String model = getModel();
	model.toCharArray(modelName, 16);
	return true;
}

void ADS1x9y::begin() {
	if (_active > -1) {
		ERROR("Cannot module %d begin on cs=%d. Module %d is already active."CR,
				_module, _cs, _active);
		return;
	}
	DEBUG("Begin module %d on cs=%d."CR, _module, _cs);
	pinSetup();
	_active = _module;

	getDeviceId();
}

void ADS1x9y::end() {
	DEBUG("End module %d on cs=%d."CR, _module, _cs);
	SPI.end();
	_active = -1;
}

int ADS1x9y::getModule() {
	return _module;
}
void ADS1x9y::RDATAC(void) {
	transfer(SPI_CMD_RDATAC);
	_continous = true;
}

void ADS1x9y::SDATAC(void) {
	transfer(SPI_CMD_SDATAC);
	_continous = false;
}

void ADS1x9y::RDATA(uint8_t* buffer, uint16_t bsize) {
	if (!_continous) {
		transfer(SPI_CMD_RDATA, true);
	}
	register uint8_t size = getTotalBytes(); // status registers + channels
	if (bsize < size) {
		ERROR("RDATA: buffer too small!");
		return;
	}

	for (uint8_t i = 0, j = size - 1; i < size; i++, j--) {
		buffer[i] = transfer(0, !!j);
	}
}

void ADS1x9y::RREG(uint8_t reg, uint8_t count, uint8_t* buffer) {
	transfer(SPI_CMD_RREG | (0x0f & reg), true); // send command and address
	delayMicroseconds(2); // for higher clockrates
	transfer(0x1f & (count - 1), true); // send count
	for (uint8_t i = 0, j = count - 1; i < count; i++, j--) {
		buffer[i] = transfer(0, !!j);
	}
	deactivate();
}

void ADS1x9y::WREG(uint8_t reg, uint8_t count, uint8_t* buffer) {
	transfer(SPI_CMD_WREG | (0x0f & reg), true); // send command and address
	delayMicroseconds(2);
	transfer(0x1f & (count - 1), true); // send count
	for (uint8_t i = 0, j = count - 1; i < count; i++, j--) {
		transfer(buffer[i], !!j);
	}
	deactivate();
}
void ADS1x9y::WAKEUP(void) {
	transfer(SPI_CMD_WAKEUP);
}
void ADS1x9y::STANDBY(void) {
	transfer(SPI_CMD_STANDBY);
}

void ADS1x9y::RESET(void) {
	transfer(SPI_CMD_RESET);
}

void ADS1x9y::START(void) {
	transfer(SPI_CMD_START);
}

void ADS1x9y::STOP(void) {
	transfer(SPI_CMD_STOP);
}

uint8_t ADS1x9y::getRegister(uint16_t reg) {
	uint8_t r;
	RREG(reg, 1, &r);
	return r;
}

void ADS1x9y::setRegister(uint16_t reg, uint8_t data) {
	WREG(reg, 1, &data);
}

void ADS1x9y::activate(void) {
	digitalWrite(_cs, LOW);
	while (!isActive())
		delayMicroseconds(2);
}

void ADS1x9y::deactivate(void) {
	digitalWrite(_cs, HIGH);
	while (isActive())
		delayMicroseconds(2);
}

boolean ADS1x9y::isActive(void) {
	return digitalRead(_cs) == LOW;
}

uint8_t ADS1x9y::transfer() {
	return transfer(0);
}

uint8_t ADS1x9y::transfer(uint8_t data) {
	return transfer(data, false);
}

uint8_t ADS1x9y::transfer(uint8_t data, boolean hold) {
	if (!isActive()) {
		activate();
	}
	register uint8_t r = SPI.transfer(data);
	if (!hold) {
		deactivate();
	}
	return r;
}

ADS1x9y::operator bool() {
	return digitalRead(PIN_DRDY_) == LOW;
}

void ADS1x9y::start() {
	digitalWrite(PIN_START, HIGH);
	DEBUG("Start"CR);
}

void ADS1x9y::stop() {
	digitalWrite(PIN_START, LOW);
	DEBUG("Stop"CR);
}

void ADS1x9y::reset() {
	digitalWrite(PIN_RESET_, LOW);
	delayMicroseconds(20);
	digitalWrite(PIN_RESET_, HIGH);
	DEBUG("Reset"CR);
}

void ADS1x9y::pwrdn() {
	DEBUG("Power down"CR);
	digitalWrite(PIN_PWDN_, LOW);
}

void ADS1x9y::pwrup() {
	DEBUG("Power up"CR);
	digitalWrite(PIN_PWDN_, HIGH);
}

void ADS1x9y::gpioMode(uint8_t pin, int mode) {
	pinMode(GPIO(pin), mode);
}

int ADS1x9y::gpioRead(uint8_t pin) {
	return digitalRead(GPIO(pin));
}

void ADS1x9y::gpioWrite(uint8_t pin, int value) {
	digitalWrite(GPIO(pin), value);
}

void ADS1x9y::pinSetup() {

	if (!_pinSet) {
		pinMode(PIN_DRDY_, INPUT);
		//pinMode(PIN_MOSI, OUTPUT);
		//pinMode(PIN_MISO, INPUT);
		//pinMode(PIN_SCLK, OUTPUT);

		pinMode(PIN_GPIO1, INPUT);
		pinMode(PIN_GPIO2, INPUT);
		pinMode(PIN_GPIO3, INPUT);
		pinMode(PIN_GPIO4, INPUT);

		pinMode(PIN_ICLK, INPUT);

		pinMode(PIN_RESET_, OUTPUT);
		pinMode(PIN_START, OUTPUT);
		pinMode(PIN_PWDN_, OUTPUT);

		digitalWrite(PIN_START, LOW);
		digitalWrite(PIN_PWDN_, HIGH);
		digitalWrite(PIN_RESET_, HIGH);

		delay(100);

		digitalWrite(PIN_RESET_, LOW);
		delayMicroseconds(20);
		digitalWrite(PIN_RESET_, HIGH);

		delayMicroseconds(20);

		ssiSetup(0);

		_pinSet = true;
	}

	pinMode(_cs, OUTPUT);
	digitalWrite(_cs, HIGH);

	// send SDATAC
	if (_continous)
		SDATAC();
	delayMicroseconds(20);

}

void ADS1x9y::getDeviceId() {
	if (_channels)
		return;
	uint8_t id;
	RREG(0, 1, &id);
	switch (id & 0xe0) {
	case 5: // ADS119x
		_channels &= 0xcf;
		break;
	case 6: // ADS129xR
		_channels |= 0x20;
	case 4: // ADS129x
		_channels |= 0x10;
		break;
	}
	switch (id & 0x03) {
	case 0: // x=4
		_channels = 0x04 | (_channels & 0xf0);
		break;
	case 1: // x=6
		_channels = 0x06 | (_channels & 0xf0);
		break;
	case 2: // x=8
		_channels = 0x08 | (_channels & 0xf0);
		break;
	case 3: //reserved
		break;
	}
}
String ADS1x9y::getModel(void) {
	String model = "ADS";
	if (_channels & 0x10) {
		model += "129";
	} else {
		model += "119";
	}
	model += _channels & 0x0f;
	if (_channels & 0x20) {
		model += 'R';
	}
	return model;
}

uint8_t ADS1x9y::getChannelCount(void) {
	return _channels & 0x0f;
}

uint8_t ADS1x9y::getSampleBits(void) {
	return _channels & 0x10 ? 24 : 16;
}

uint8_t ADS1x9y::getSampleBytes(void) {
	return _channels & 0x10 ? 3 : 2;
}
uint16_t ADS1x9y::getTotalBytes(void) {
	return getSampleBytes() * getChannelCount() + 3;
}

void ADS1x9y::ssiSetup(uint8_t SSIModule) {
	SPI.setModule(SSIModule);
	SPI.begin();
	//SPI.setClockDivider(SPI_CLOCK_DIV2);
	SPI.setDataMode(SPI_MODE1);

}

ADS1x9y analogSensor = ADS1x9y();
