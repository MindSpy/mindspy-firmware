#include "SensorHandler.h"

#include <stdio.h>
#include "regs_pb.h"
#include "Sensor.h"
#include "macros.h"

#define LOGLEVEL LOG_LEVEL_VERBOSE
#include "Logging.h"

bool SensorHandler::handle(Request* request, Response* response, pb_istream_t* istream, pb_ostream_t* ostream) {

    // intercept request message
    if (!(*_decoder)(istream, Request_fields, request)) {
        response->has_error_msg = true;
        snprintf(response->error_msg, COUNT_OF(response->error_msg), "Decoding of request message failed.");
        return false;
    }

     DEBUG("<- {reqid=%i}"CR, request->reqid);

    // issue response one or more times
    do {
        //defaults
        response->has_error_msg = false;
        response->has_module = false;
        response->reqid = request->reqid;

        ISensor* sensor = NULL;
        uint8_t module = 0;

        // obtain module number from request
        if (request->has_module) {
            module = request->module;
        }

        // invoke for each module - exit loop when module is explicitly specified in request
        do {
            response->has_module = true;
            response->module = module;

            if (_timestamp != NULL) {
                response->timestamp = (*_timestamp)();
            }

            if (module >= _sensorCount) {
                if (request->has_module) {
                    response->has_error_msg = true;
                    snprintf(response->error_msg, COUNT_OF(response->error_msg), "Requested module #%d is outside of interval [0,%d].", module, _sensorCount-1);
                    return false;
                }
            }

            // invoke ISensor methods based on request

            if (request->has_setState || request->has_getState || request->has_getSamples || request->has_getModelName) {

                sensor = _sensors[module];

                if (request->has_setState) {
                    if (!sensor->setState(request->setState.states, request->setState.states_count, NULL)) {
                        response->has_error_msg = true;
                        snprintf(response->error_msg, COUNT_OF(response->error_msg), "Method setState of module #%d failed.", module);
                        return false;
                    }
                }

                if (request->has_getState) {
                    if (!sensor->getState(request->getState.addresses, request->getState.addresses_count, response->states)) {
                        response->has_error_msg = true;
                        snprintf(response->error_msg, COUNT_OF(response->error_msg), "Method getState of module #%d failed.", module);
                        return false;
                    }
                    response->states_count = request->getState.addresses_count;
                }

                if (request->has_getSamples) {
                    if (!sensor->getSamples(request->getSamples.count, response->samples)) {
                        response->has_error_msg = true;
                        snprintf(response->error_msg, COUNT_OF(response->error_msg), "Method getSamples of module #%d failed.", module);
                        return false;
                    }
                    response->samples_count = request->getSamples.count;
                }

                if (request->has_getModelName) {
                    if (!sensor->getModelName(response->modelName)) {
                        response->has_error_msg = true;
                        snprintf(response->error_msg, COUNT_OF(response->error_msg), "Method getModelName of module #%d failed.", module);
                        return false;
                    }
                    response->has_modelName = true;
                }
            }

            // serialize response message

            if (!(*_encoder)(ostream, Response_fields, response)) {
                response->has_error_msg = true;
                snprintf(response->error_msg, COUNT_OF(response->error_msg), "Encoding of response message failed.");
                return false;
            }

            DEBUG("-> {reqid=%i}"CR, request->reqid);

        } while (!request->has_module && (++module < _sensorCount));
    } while (request->stream && (!(*_stop)()));

    return true;

}

