#ifndef SENDORDETECTOR_HPP
#define SENDORDETECTOR_HPP

#include "Sensor.h"

/*!
 * \brief The SensorDetector class
 */
class SensorDetector {
public:

    /*!
     * \brief SensorDetector
     * \param sensors
     * \param count of sensors
     */
    SensorDetector(sensor::Sensor**, size_t);
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
    /*!
     * \brief countOfSensors
     */
    unsigned int countOfSensors;
};

#endif // SENDORDETECTOR_H