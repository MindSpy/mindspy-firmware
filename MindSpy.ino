
#define LOGLEVEL LOG_LEVEL_VERBOSE


#include <Arduino.h>
#include <SPI.h>
#include <Logging.h>
#include <pb.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include "ADS1x9y.h"
#include "regs_pb.h"
#include "pb_common.h"
#include "Main.h"
