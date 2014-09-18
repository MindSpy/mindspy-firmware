/*
TI ADS1x9y Analog Frontend 

created 30 Nov 2013
by pBorky
*/

#include <Arduino.h>
#include <SPI.h>
#include <Logging.h>
#include "Sensor.h"

#ifndef ADS1x9y_h
#define ADS1x9y_h

/*!
 * TI ADS1x9y module.
 * See <a href="http://www.ti.com/lit/ds/symlink/ads1298.pdf">ADS1298</a> for details.
 */
class ADS1x9y: public ISensor {
  private:    
    int _module;
    int _cs;
    uint8_t _channels;
    boolean _continous;
    
    static boolean _pinSet;
    static int _active;
    static int _sequence;
    
    void pinSetup(void);
    void getDeviceId(void);
    
    static void ssiSetup(uint8_t SSIModule);
  public:
    /*!
     * Constructor.
     * \param module nnumber (CS pin)
     */
    ADS1x9y(uint8_t);
    
    // override ISensor methods
    bool getSamples(uint32_t, Sample*);
    bool getState(uint32_t*, uint32_t, State*);
    bool setState(State*, uint32_t, void*);
    bool getModelName(char*);
    
    /*!
     * Activate this module if possible and setup SPI.
     */ 
    void begin();
    /*!
     * Deativate this module if possible and stop SPI.
     */ 
    void end();
    /*!
     * Get module id
     */
    int getModule();
    
    //module operations
    
    /*!
     * Read data (cnotinous). 
     * See <a href="http://www.ti.com/lit/ds/symlink/ads1298.pdf">ADS1298</a> for details.
     */
    void RDATAC(void);
    /*!
     * Stop data read (continous). 
     * See <a href="http://www.ti.com/lit/ds/symlink/ads1298.pdf">ADS1298</a> for details.
     */
    void SDATAC(void);
    /*!
     * Read data.
     * See <a href="http://www.ti.com/lit/ds/symlink/ads1298.pdf">ADS1298</a> for details.
     */
    void RDATA(uint8_t*,uint16_t);
    /*!
     * Read register. 
     * See <a href="http://www.ti.com/lit/ds/symlink/ads1298.pdf">ADS1298</a> for details.
     */
    void RREG(uint8_t,uint8_t,uint8_t*);
    /*!
     * Write register. 
     * See <a href="http://www.ti.com/lit/ds/symlink/ads1298.pdf">ADS1298</a> for details.
     */
    void WREG(uint8_t,uint8_t,uint8_t*);
    /*!
     * Wakeup from stanby. 
     * See <a href="http://www.ti.com/lit/ds/symlink/ads1298.pdf">ADS1298</a> for details.
     */
    void WAKEUP(void);
    /*!
     * Standby. 
     * See <a href="http://www.ti.com/lit/ds/symlink/ads1298.pdf">ADS1298</a> for details.
     */
    void STANDBY(void);
    /*!
     * Reset the single module via command. 
     * See <a href="http://www.ti.com/lit/ds/symlink/ads1298.pdf">ADS1298</a> for details.
     */
    void RESET(void);
    /*!
     * Start conversion in single module via command. 
     * See ADS1x9y::start() and <a href="http://www.ti.com/lit/ds/symlink/ads1298.pdf">ADS1298</a> for details.
     */
    void START(void);
    /*!
     * Stop conversion in single module via command. 
     * See ADS1x9y::stop() and <a href="http://www.ti.com/lit/ds/symlink/ads1298.pdf">ADS1298</a> for details.
     */
    void STOP(void);
    
    // high level operations
    
    /*!
     * Get content of the register.
     * \param register to get
     * \return value
     */
    uint8_t getRegister(uint16_t);
    /*!
     * Set content of the register.
     * \param register to set
     * \param value
     */
    void setRegister(uint16_t, uint8_t);
    /*!
     * Get model name.
     * \return model name
     */
    String getModel(void);
    /*!
     * Get channel count.
     * \return number of channels
     */
    uint8_t getChannelCount(void);
    /*!
     * Get sample size in bits.
     * \return number of bits in sample in one channel
     */
    uint8_t getSampleBits(void); // in bits
    /*!
     * Get sample size in bytes.
     * \return number of bytes in sample in one channel
     */
    uint8_t getSampleBytes(void); 
    /*!
     * Get total number of bytes of one sample over all channels.
     * \return number of bytes in samples
     */
    uint16_t getTotalBytes(void); 
    
    // low level operations
    
    /*!
     * Transfer zero to the slave and return the response.
     * \return result returned by module
     */
    uint8_t transfer(void);
    
    /*!
     * Transfer one byte to the slave and return the response.
     * \param byte to transfer
     * \return result returned by module
     */
    uint8_t transfer(uint8_t);
    
    /*! 
     * Transfer one byte to the slave and return the response.
     * \param byte to transfer
     * \param hold the CS line (for multi-byte operations)
     * \return result returned by module
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
    
    /*! 
     * Check if data is ready in this module.
     */
    operator bool(void);
    /*!
     * Start conversion in all modules via pin. 
     * See ADS1x9y::START() and <a href="http://www.ti.com/lit/ds/symlink/ads1298.pdf">ADS1298</a> for details.
     */
    static void start(void);
    /*!
     * Stop conversion in all modules via pin. 
     * See ADS1x9y::STOP() and <a href="http://www.ti.com/lit/ds/symlink/ads1298.pdf">ADS1298</a> for details.
     */
    static void stop(void);
    /*!
     * Reset all modules via pin.
     * See ADS1x9y::RESET() and <a href="http://www.ti.com/lit/ds/symlink/ads1298.pdf">ADS1298</a> for details.
     */
    static void reset(void);
    /*!
     * Power down all modules via pin. 
     * See <a href="http://www.ti.com/lit/ds/symlink/ads1298.pdf">ADS1298</a> for details.
     */
    static void pwrdn(void);
    /*!
     * Power up all modules via pin. 
     * See <a href="http://www.ti.com/lit/ds/symlink/ads1298.pdf">ADS1298</a> for details.
     */
    static void pwrup(void);
    /*!
     * Set mode of GPIO pin. 
     * See <a href="http://www.ti.com/lit/ds/symlink/ads1298.pdf">ADS1298</a> for details.
     * \param GPIO number
     * \param mode
     */
    static void gpioMode(uint8_t, int);
    /*!
     * Read value of GPIO pin. 
     * See <a href="http://www.ti.com/lit/ds/symlink/ads1298.pdf">ADS1298</a> for details.
     * \param GPIO number
     * \result value
     */
    static int gpioRead(uint8_t);
    /*!
     * Write value of GPIO pin. 
     * See <a href="http://www.ti.com/lit/ds/symlink/ads1298.pdf">ADS1298</a> for details.
     * \param GPIO number
     * \param new value
     */
    static void gpioWrite(uint8_t, int);
};

extern ADS1x9y  Sensor;
#endif
