/*
ADS1x9y Analog Frontend 

created 30 Nov 2013
by pBorky
*/

#include <Arduino.h>
#include <SPI.h>
#include <Logging.h>

#ifndef ADS1x9y_h
#define ADS1x9y_h

class ADS1x9y {
  private:    
    int _module;
    int _cs;
    uint8_t _channels;
    boolean _continous;
    
    static boolean _pinSet;
    static int _active;
    
    void pinSetup(void);
    void getDeviceId(void);
    
    static void ssiSetup(uint8_t SSIModule);
  public:
    ADS1x9y(uint8_t);
    
    void begin();
    void end();
    
    // high level operations
    void RDATAC(void);
    void SDATAC(void);
    void RDATA(uint8_t*,uint16_t);
    void RREG(uint8_t,uint8_t,uint8_t*);
    void WREG(uint8_t,uint8_t,uint8_t*);
    void WAKEUP(void);
    void STANDBY(void);
    void RESET(void);
    void START(void);
    void STOP(void);
     
    uint8_t getRegister(uint16_t);
    void setRegister(uint16_t, uint8_t);
    String getModel(void);
    uint8_t getChannelCount(void);
    uint8_t getSampleBits(void); // in bits
    uint8_t getSampleBytes(void); 
    uint16_t getTotalBytes(void); 
    
    // low level operations
    
    /*!
     * Transfer zero to the slave and return the response.
     */
    uint8_t transfer(void);
    
    /*!
     * Transfer one byte to the slave and return the response.
     */
    uint8_t transfer(uint8_t);
    
    /*! 
     * Transfer one byte to the slave and return the response.
     */
    uint8_t transfer(uint8_t,boolean);
    
    /*! 
     * Activate the slave select port.
     */
    void activate(void);
    
    /*!
     * Deactivate the slave select port.
     */
    void deactivate(void);
    
    /*! 
     * Determine whether slave select is active.
     */
    boolean isActive(void);
    

    // pin operations
     
    operator bool(void);
    static void start(void);
    static void stop(void);
    static void reset(void);
    static void pwrdn(void);
    static void pwrup(void);
    static void gpioMode(uint8_t, int);
    static int gpioRead(uint8_t);
    static void gpioWrite(uint8_t, int);
};

extern ADS1x9y  Sensor;
#endif
