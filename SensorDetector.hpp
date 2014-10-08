#ifndef SENDORDETECTOR_HPP
#define SENDORDETECTOR_HPP

#include "Sensor.hpp"
#include "Sensors.hpp"
#include <vector>

/*!
 * \brief The SensorDetector class
 */
class SensorDetector : public Sensors {
public:

    /*!
     * \brief SensorDetector
     * \param sensors
     * \param count of sensors
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
    sensor::Sensor*& operator[](size_t idx);

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
