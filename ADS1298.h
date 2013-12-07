/*
ADS1298 Analog Frontend 

created 30 Nov 2013
by pBorky
*/

#ifndef ADS1298_h
#define ADS1298_h

class ADS1298 {
  public:
    ADS1298();
    ADS1298(int module);
    void begin();
    void end();
    byte getRegister(int reg);
    void setRegister(int reg, byte data);
    static void start();
    static void stop();
    static void reset();
    static void pwrdn();
    static void pwrup();
    static void gpioMode(int pin, int mode);
    static int gpioRead(int pin);
    static void gpioWrite(int pin, int value);
  private:
    void pinSetup();
    int _module;
    int _cs;
    static boolean pinSet;
    static SPIClass spi;
};

#endif
