#ifndef TESTSENSOR_HPP
#define TESTSENSOR_HPP

#include "Sensor.hpp"
#include "proto.h"

namespace mindspy {

namespace sensor {

/*!
 * \brief TestSensor for unit tests
 */
class TestSensor : public mindspy::sensor::Sensor {

public:

    /*!
     * \brief Contructor
     * \param model name of the sensor
     * \param rate of generated waveform
     * \param number of generated channels
     */
    TestSensor(const char*, const uint8_t, const uint8_t);

    // override Sensor methods
    bool getSamples(uint32_t, mindspy_protobufs_Sample*);
    bool getState(uint32_t*, uint32_t, mindspy_protobufs_State*);
    bool setState(mindspy_protobufs_State*, uint32_t, void*);
    bool getModelName(char*);
    bool operator!(void);
    void begin();
    void end();

private:

    const char * name;
    uint64_t sequence;
    uint8_t rate;
    uint8_t channels;
    unsigned long lastTime;

    uint8_t getState(uint32_t);
    bool setState(mindspy_protobufs_State state);

};

} // namespace

} // namespace
#endif
