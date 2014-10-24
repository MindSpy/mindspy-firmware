
#include "ADS1x9y.hpp"
#include "Sensor.hpp"
#include <Arduino.h>

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

bool ADS1x9y::pinSet = false;
int ADS1x9y::active = -1;
int ADS1x9y::sequence = 0;

ADS1x9y::ADS1x9y(uint8_t module) :
    module(constrain(module, 0, SPI_CS_PIN_COUNT - 1)), cs(SPI_CS(module)), channels(0), continous(true)
{
}

ADS1x9y::ADS1x9y() :
    module(0), cs(SPI_CS(module)), channels(0), continous(true)
{
}

bool ADS1x9y::getSamples(uint32_t count, mindspy_protobufs_Sample* result) {

    uint8_t channels = this->getChannelCount();
    uint8_t bytes = this->getSampleBytes();
    uint16_t bsize = this->getTotalBytes();
    uint8_t* buffer = (uint8_t*) malloc(bsize * sizeof(uint8_t));

    for (uint32_t i = 0; i < count; i++) {
        mindspy_protobufs_Sample* samp = &(result[i]);

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
        samp->sequence = sequence++;
        samp->payload_count = channels;

    }
    free(buffer);
    SDATAC();
    return true;
}

bool ADS1x9y::getState(uint32_t* addresses, uint32_t addresses_count, mindspy_protobufs_State* result) {
    uint8_t buffer = 0;

    for (uint32_t i = 0; i < addresses_count; i++) {
        this->RREG(addresses[i], 1, &buffer);

        result[i].address = addresses[i];
        result[i].payload = buffer;
    }

    return true;
}

bool ADS1x9y::setState(mindspy_protobufs_State* states, uint32_t states_count, void* result) {
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
    if (active > -1) {
        return;
    }
    pinSetup();
    active = module;

    getDeviceId();
    START();
}

void ADS1x9y::end() {
    spi.end();
    active = -1;
}

int ADS1x9y::getModule() {
    return module;
}
void ADS1x9y::RDATAC(void) {
    transfer(SPI_CMD_RDATAC);
    continous = true;
}

void ADS1x9y::SDATAC(void) {
    transfer(SPI_CMD_SDATAC);
    continous = false;
}

void ADS1x9y::RDATA(uint8_t* buffer, uint16_t bsize) {
    if (!continous) {
        transfer(SPI_CMD_RDATA, true);
    }
    register uint8_t size = getTotalBytes(); // status registers + channels
    if (bsize < size) {
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
    digitalWrite(cs, LOW);
    while (!isActive())
        delayMicroseconds(2);
}

void ADS1x9y::deactivate(void) {
    digitalWrite(cs, HIGH);
    while (isActive())
        delayMicroseconds(2);
}

bool ADS1x9y::isActive(void) {
    return digitalRead(cs) == LOW;
}

uint8_t ADS1x9y::transfer() {
    return transfer(0);
}

uint8_t ADS1x9y::transfer(uint8_t data) {
    return transfer(data, false);
}

uint8_t ADS1x9y::transfer(uint8_t data, bool hold) {
    if (!isActive()) {
        activate();
    }
    register uint8_t r = spi.transfer(data);
    if (!hold) {
        deactivate();
    }
    return r;
}

bool ADS1x9y::operator!() {
    return digitalRead(PIN_DRDY_) != LOW;
}

void ADS1x9y::start() {
    digitalWrite(PIN_START, HIGH);
}

void ADS1x9y::stop() {
    digitalWrite(PIN_START, LOW);
}

void ADS1x9y::reset() {
    digitalWrite(PIN_RESET_, LOW);
    delayMicroseconds(20);
    digitalWrite(PIN_RESET_, HIGH);
}

void ADS1x9y::pwrdn() {
    digitalWrite(PIN_PWDN_, LOW);
}

void ADS1x9y::pwrup() {
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

    if (!pinSet) {
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

        pinSet = true;
    }

    pinMode(cs, OUTPUT);
    digitalWrite(cs, HIGH);

    // send SDATAC
    if (continous)
        SDATAC();
    delayMicroseconds(20);

}

void ADS1x9y::getDeviceId() {
    if (channels)
        return;
    uint8_t id;
    RREG(0, 1, &id);
    switch (id & 0xe0) {
    case 5: // ADS119x
        channels &= 0xcf;
        break;
    case 6: // ADS129xR
        channels |= 0x20;
    case 4: // ADS129x
        channels |= 0x10;
        break;
    }
    switch (id & 0x03) {
    case 0: // x=4
        channels = 0x04 | (channels & 0xf0);
        break;
    case 1: // x=6
        channels = 0x06 | (channels & 0xf0);
        break;
    case 2: // x=8
        channels = 0x08 | (channels & 0xf0);
        break;
    case 3: //reserved
        break;
    }
}
String ADS1x9y::getModel(void) {
    String model = "ADS";
    if (channels & 0x10) {
        model += "129";
    } else {
        model += "119";
    }
    model += channels & 0x0f;
    if (channels & 0x20) {
        model += 'R';
    }
    return model;
}

uint8_t ADS1x9y::getChannelCount(void) {
    return channels & 0x0f;
}

uint8_t ADS1x9y::getSampleBits(void) {
    return channels & 0x10 ? 24 : 16;
}

uint8_t ADS1x9y::getSampleBytes(void) {
    return channels & 0x10 ? 3 : 2;
}
uint16_t ADS1x9y::getTotalBytes(void) {
    return getSampleBytes() * getChannelCount() + 3;
}

void ADS1x9y::ssiSetup(uint8_t SSIModule) {
    spi.setModule(SSIModule);
    //spi.setClockDivider(SPI_CLOCK_DIV2);
    spi.setDataMode(SPI_MODE1);
    spi.begin();
}
