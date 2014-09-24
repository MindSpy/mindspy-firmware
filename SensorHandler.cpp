#include "SensorHandler.h"
#include <pb_encode.h>
#include <pb_decode.h>
#include <pb.h>
#include "regs_pb.h"
#include "Sensor.h"
#include "macros.h"

#define LOGLEVEL LOG_LEVEL_VERBOSE
#include "Logging.h"

namespace sensor {

SensorHandler::SensorHandler(TimestampCallbackType time, StopStreamCallbackType stop, sensor::Sensor** sensor, size_t sensors) :
		timesStamp(time), stopStream(stop), sensors(sensor), boardSensor(NULL), countOfSensors(sensors) {
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

	DEBUG("<- {reqid=%i}"CR, request.reqid);

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

			if (timesStamp != NULL) {
				response.timestamp = (*timesStamp)();
			}

			if (module >= countOfSensors) {
				if (request.has_module) {
					response.has_error_msg = true;
					snprintf(response.error_msg, COUNT_OF(response.error_msg),
							"Requested module #%d is outside of interval [0,%d].",
							module, countOfSensors - 1);
					return false;
				}
			}

			// invoke Sensor methods based on request

			if (request.has_setState || request.has_getState
					|| request.has_getSamples || request.has_getModelName) {

				boardSensor = sensors[module];

				if (request.has_setState) {
					if (!boardSensor->setState(request.setState.states,
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
					if (!boardSensor->getState(request.getState.addresses,
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
					if (!boardSensor->getSamples(request.getSamples.count,
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
					if (!boardSensor->getModelName(response.modelName)) {
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
				if (response.has_error_msg)
					ERROR("Previous error: %s"CR, response.error_msg);
				ERROR("Encoding of response message failed."CR);
				return false;
			}

			DEBUG("-> {reqid=%i}"CR, request.reqid);

		} while (!request.has_module && (++module < countOfSensors));
	} while (request.stream && (!(*stopStream)()));

	return true;

}

} // namespace
