
#include "regs_pb.h"


#ifndef Sensor_h
#define Sensor_h

class ISensor {
  public:
    virtual ~ISensor() {};
    /*!
     * Get Samples from the sensor
     * \param count of samples to get
     * \return result
     */
    virtual bool getSamples(uint32_t, Sample*) = 0;
    
    /*!
     * Get State of the sensor (get the registers).
     * \param addresses to get
     * \return result states
     */
    virtual bool getState(uint32_t*, uint32_t, State*) = 0;
    
    /*!
     * Set State of the sensor (set the registers).
     * \param states to set up
     */
    virtual bool setState(State*, uint32_t, void*) = 0;
    
    /*!
     * Get model name.
     * \param result model name
     */
    virtual bool getModelName(char*) = 0;
};

#endif
