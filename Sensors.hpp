#ifndef SENSORS_H
#define SENSORS_H

#include <vector>
#include "Sensor.hpp"

namespace mindspy {

namespace sensor {

class Sensors {
protected:

    typedef std::vector<sensor::Sensor*> container;

public:

    virtual ~Sensors() {
    }

    typedef container::iterator iterator;

    /*!
     * \brief begin
     * \return
     */
    virtual iterator begin() = 0;

    /*!
     * \brief end
     * \return
     */
    virtual iterator end() = 0;

    /*!
     * \brief count
     * \return return number of sensors registered
     */
    virtual size_t count() = 0;

    /*!
     * \brief operator []
     * \param idx
     * \return
     */
    virtual mindspy::sensor::Sensor*& operator[](const size_t idx) = 0;
};

} // namespace

} // namespace

#endif // SENSORS_H
