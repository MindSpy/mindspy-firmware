#include "SensorHandler.hpp"
#include <pb_encode.h>
#include <pb_decode.h>
#include "proto.h"
#include "Sensor.hpp"

#ifndef _UNITTEST
#include <Arduino.h>
#else
#include "compat.hpp"
#endif

namespace sensor {

    SensorHandler::SensorHandler() {
    }

    SensorHandler::~SensorHandler() {
    }

    void SensorHandler::setSensors(Sensors*s) {
        sensors = s;
    }

    void SensorHandler::setStopCallback(StopStreamCallbackType s) {
        stopStream = s;
    }

    bool SensorHandler::handle(pb_istream_t* istream, pb_ostream_t* ostream) {

        // intercept request message
        if (!pb_decode_delimited(istream, mindspy_protobufs_Request_fields, &request)) {
            response.has_error_msg = true;
            snprintf(response.error_msg,
                    DECLARED_ARRAY_ITEM_COUNT(response.error_msg),
                    "Decoding of request message failed.");
            return false;
        }

        // response defaults
        response.timestamp = 0;
        response.has_module = false;
        response.states_count = 0;
        response.samples_count = 0;
        response.has_modelName = false;
        response.has_error_msg = false;

        bool mustRespond = request.has_setState || request.has_getState ||
                request.has_getSamples || request.has_getModelName;

        // issue response one or more times
        do {
            //defaults
            response.has_error_msg = false;
            response.has_module = false;
            response.reqid = request.reqid;
            response.timestamp = micros();


            // obtain module number from request
            if (request.has_module) {
                // invoke for single module
                uint8_t module = request.module;

                if (module >= sensors->count()) {
                    if (request.has_module) {
                        response.has_error_msg = true;
                        snprintf(response.error_msg,
                                DECLARED_ARRAY_ITEM_COUNT(response.error_msg),
                                "Requested module #%u is outside of interval [0, %zu].",
                                module, sensors->count() - 1);
                        return false;
                    }
                }

                response.has_module = true;
                response.module = module;

                if (mustRespond) {
                    // invoke Sensor methods based on request
                    if (!respond((*sensors)[module], request, response))
                        return false;
                }

                // serialize response message
                if (!pb_encode_delimited(ostream, mindspy_protobufs_Response_fields, &response)) {
                    return false;
                }
            } else {
                // invoke for each module - exit loop when module is explicitly specified in request
                for (Sensors::iterator is = sensors->begin(); is != sensors->end(); is++) {

                    response.has_module = true;
                    response.module = is  - sensors->begin();

                    if (mustRespond) {
                        // invoke Sensor methods based on request
                        if (!respond(*is, request, response))
                            return false;
                    }

                    // serialize response message

                    if (!pb_encode_delimited(ostream, mindspy_protobufs_Response_fields, &response)) {
                        return false;
                    }
                }
            }

        } while (request.stream && (!(*stopStream)()));

        return true;

    }

    bool SensorHandler::respond(Sensor* sensor, mindspy_protobufs_Request& request, mindspy_protobufs_Response& response ) {

        uint8_t module = (uint8_t)response.module;

        if (request.has_setState) {
            if (!sensor->setState(request.setState.states,
                    request.setState.states_count, NULL)) {
                response.has_error_msg = true;
                snprintf(response.error_msg,
                        DECLARED_ARRAY_ITEM_COUNT(response.error_msg),
                        "Method setState of module #%u failed.",
                        module);
                return false;
            }
        }

        if (request.has_getState) {
            if (!sensor->getState(request.getState.addresses,
                    request.getState.addresses_count,
                    response.states)) {
                response.has_error_msg = true;
                snprintf(response.error_msg,
                        DECLARED_ARRAY_ITEM_COUNT(response.error_msg),
                        "Method getState of module #%u failed.",
                        module);
                return false;
            }
            response.states_count =
                    request.getState.addresses_count;
        }

        if (request.has_getSamples) {
            if (!sensor->getSamples(request.getSamples.count, response.samples)) {
                response.has_error_msg = true;
                snprintf(response.error_msg,
                        DECLARED_ARRAY_ITEM_COUNT(response.error_msg),
                        "Method getSamples of module #%u failed.",
                        module);
                return false;
            }
            response.samples_count = request.getSamples.count;
        }

        if (request.has_getModelName) {
            if (!sensor->getModelName(response.modelName)) {
                response.has_error_msg = true;
                snprintf(response.error_msg,
                        DECLARED_ARRAY_ITEM_COUNT(response.error_msg),
                        "Method getModelName of module #%u failed.",
                        module);
                return false;
            }
            response.has_modelName = true;
        }
        return true;
    }

} // namespace
