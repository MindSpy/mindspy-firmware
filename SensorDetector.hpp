#ifndef SENDORDETECTOR_HPP
#define SENDORDETECTOR_HPP

#include <vector>
#include "Sensor.h"

/*!
 * The SensorDetector class detection sensors.
 */
class SensorDetector {
public:

    SensorDetector(std::vector<sensor::Sensor*> &sensors);
    ~SensorDetector();

    /*!
     * \brief count
     * \return return number of sensors registered
     */
    size_t count() const { return sensors.size(); }

    /*!
     * \brief operator [] acces the member
     * \param idx index of the memeber to access
     * \return idx-th memeber
     */
    sensor::Sensor*& operator[](size_t idx);

private:

    std::vector<sensor::Sensor*> &sensors;
};

#endif // SENDORDETECTOR_H