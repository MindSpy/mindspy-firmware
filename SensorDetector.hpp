#ifndef SENDORDETECTOR_HPP
#define SENDORDETECTOR_HPP

#include "Sensor.hpp"
#include "Sensors.hpp"
#include <vector>

/*!
 * The SensorDetector class detection sensors.
 */
class SensorDetector : public Sensors {
public:

    /*!
     * \brief SensorDetector
     */
    SensorDetector();
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
    sensor::Sensor*& operator[](const size_t idx);

    /*!
     * \brief begin
     * \return
     */
    iterator begin();

    /*!
     * \brief end
     * \return
     */
    iterator end();

private:
    /*!
     * \brief sensors
     */
    container sensors;
};

#endif // SENDORDETECTOR_H
