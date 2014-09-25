#include "SensorHandler.h"
#include <pb_encode.h>
#include <pb_decode.h>
#include "proto.h"
#include "Sensor.h"
#include "macros.h"

#ifndef _UNITTEST

#include <Arduino.h>

#else

#include <sys/time.h>

// micros for unit testing (wiring is not available)
uint64_t micros() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return tp.tv_sec * 1000000 + tp.tv_usec;
}

#endif

namespace sensor {

SensorHandler::SensorHandler(StopStreamCallbackType stop, SensorDetector* sensorDetector) :
        stopStream(stop), sensors(sensorDetector) {
}

SensorHandler::~SensorHandler() {
}

bool SensorHandler::handle(pb_istream_t* istream, pb_ostream_t* ostream) {

    // intercept request message
    if (!pb_decode_delimited(istream, Request_fields, &request)) {
        response.has_error_msg = true;
        snprintf(response.error_msg, COUNT_OF(response.error_msg),
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

    // issue response one or more times
    do {
        //defaults
        response.has_error_msg = false;
        response.has_module = false;
        response.reqid = request.reqid;

        uint8_t module = 0;

        // obtain module number from request
        if (request.has_module) {
            module = request.module;
        }

        // invoke for each module - exit loop when module is explicitly specified in request
        do {
            response.has_module = true;
            response.module = module;

            response.timestamp = micros();

            if (module >= sensors->count()) {
                if (request.has_module) {
                    response.has_error_msg = true;
                    snprintf(response.error_msg, COUNT_OF(response.error_msg),
                            "Requested module #%d is outside of interval [0, %zu].",
                            module, sensors->count() - 1);
                    return false;
                }
            }

            // invoke Sensor methods based on request

            if (request.has_setState || request.has_getState
                    || request.has_getSamples || request.has_getModelName) {

                sensor::Sensor* sensor = (*sensors)[module];

                if (request.has_setState) {
                    if (!sensor->setState(request.setState.states,
                            request.setState.states_count, NULL)) {
                        response.has_error_msg = true;
                        snprintf(response.error_msg,
                                COUNT_OF(response.error_msg),
                                "Method setState of module #%d failed.",
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
                                COUNT_OF(response.error_msg),
                                "Method getState of module #%d failed.",
                                module);
                        return false;
                    }
                    response.states_count = request.getState.addresses_count;
                }

                if (request.has_getSamples) {
                    if (!sensor->getSamples(request.getSamples.count,
                            response.samples)) {
                        response.has_error_msg = true;
                        snprintf(response.error_msg,
                                COUNT_OF(response.error_msg),
                                "Method getSamples of module #%d failed.",
                                module);
                        return false;
                    }
                    response.samples_count = request.getSamples.count;
                }

                if (request.has_getModelName) {
                    if (!sensor->getModelName(response.modelName)) {
                        response.has_error_msg = true;
                        snprintf(response.error_msg,
                                COUNT_OF(response.error_msg),
                                "Method getModelName of module #%d failed.",
                                module);
                        return false;
                    }
                    response.has_modelName = true;
                }
            }

            // serialize response message

            if (!pb_encode_delimited(ostream, Response_fields, &response)) {
                return false;
            }

        } while (!request.has_module && (++module < sensors->count()));
    } while (request.stream && (!(*stopStream)()));

    return true;

}

} // namespace
