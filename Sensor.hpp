#ifndef SENSOR_HPP
#define SENSOR_HPP

#include "proto.h"

namespace mindspy {

namespace sensor {

    class Sensor {
    public:
        virtual ~Sensor() {
        }

        /*!
         * \brief Get Samples from the sensor.
         * \param count of samples to get
         * \return result
         */
        virtual bool getSamples(uint32_t, mindspy_protobufs_Sample*) = 0;

        /*!
         * \brief Get State of the sensor (get the registers).
         * \param addresses to get
         * \param addresses count
         * \return result states
         */
        virtual bool getState(uint32_t*, uint32_t, mindspy_protobufs_State*) = 0;

        /*!
         * \brief Set State of the sensor (set the registers).
         * \param states to set up
         */
        virtual bool setState(mindspy_protobufs_State*, uint32_t, void*) = 0;

        /*!
         * \brief Get model name.
         * \param result model name
         */
        virtual bool getModelName(char*) = 0;

        /*!
         * \brief Check if data is ready in this module.
         */
        virtual bool operator!(void) = 0;

        /*!
         * \brief begin
         */
        virtual void begin() = 0;

        /*!
         * \brief end
         */
        virtual void end() = 0;
    };

} // namespace

} // namespace

#endif
