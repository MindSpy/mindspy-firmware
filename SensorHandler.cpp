
#include "SensorHandler.h"
#include <stdio.h> // TODO: deprecated - searching for lightweight sprintf

ISensor** SensorHandler::_sensors;
size_t SensorHandler::_sensorCount;
ResponseEncoderCallbackType SensorHandler::_encoder;
TimestampCallbackType SensorHandler::_timestamp;

template <size_t N>
void SensorHandler::init(const ISensor* (& sensors) [N], ResponseEncoderCallbackType encoder, TimestampCallbackType timestamp) {
    _sensors = sensors;
    _sensorCount = N;
    _encoder = encoder;
    _timestamp = timestamp;
}

bool SensorHandler::handleRequest(Request* request, Response* response) {

    //defaults
    response->has_error_msg = false;
    //response->has_module = false;
    response->reqid = request->reqid;

    ISensor* sensor = NULL;
    uint8_t module = 0;

    // obtain module number from request
    if (request->has_module) {
        module = request->module;
    }

    // invoke for each module - exit loop when module is explicitly specified in request
    do {
        //response->has_module = true;
        response->module = module;

        if (_timestamp != NULL) //TODO this can come from the caller
            response->timestamp = (*_timestamp)();

        // invoke ISensor methods based on reuest

        if (module < _sensorCount) {

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
        } else {
            if (request->has_module) {
                response->has_error_msg = true;
                snprintf(response->error_msg, COUNT_OF(response->error_msg), "Requested module #%d is outside of interval [0,%d].", module, _sensorCount-1);
                return false;
            }
        }

        // serialize response message

        if (!(*_encoder)(Response_fields, response)) {
            response->has_error_msg = true;
            snprintf(response->error_msg, COUNT_OF(response->error_msg), "Encoding of response message failed.");
            return false;
        }

    } while (!request->has_module && (++module < _sensorCount));

    return true;

}

