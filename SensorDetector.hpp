#ifndef SENDORDETECTOR_HPP
#define SENDORDETECTOR_HPP

#include <vector>
#include "Sensor.h"

/*!
 * The SensorDetector class detection sensors.
 */
class SensorDetector {
public:

    SensorDetector(std::vector<sensor::Sensor*> &sensor);
    ~SensorDetector();

    /*!
     * \brief count
     * \return return number of sensors registered
     */
    size_t count();

    /*!
     * \brief operator [] acces the member
     * \param idx index of the memeber to access
     * \return idx-th memeber
     */
    sensor::Sensor*& operator[](size_t idx);

private:
    /*!
     * \brief sensors
     */
    sensor::Sensor** sensors;

    std::vector<sensor::Sensor*> sensor;

    /*!
     * \brief countOfSensors
     */
    unsigned int countOfSensors;
};

#endif // SENDORDETECTOR_H