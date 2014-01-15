/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.2.6-dev at Sun Jan 12 03:45:09 2014. */

#include "regs_pb.h"

const bool Request_repeat_default = false;
const Response_ErrNo Response_error_default = Response_ErrNo_NO_ERROR;


const pb_field_t Separator_fields[2] = {
    PB_FIELD2(  1, BOOL    , OPTIONAL, STATIC  , FIRST, Separator, last, last, 0),
    PB_LAST_FIELD
};

const pb_field_t Request_fields[7] = {
    PB_FIELD2(  1, ENUM    , REQUIRED, STATIC  , FIRST, Request, action, action, 0),
    PB_FIELD2(  2, UINT64  , REQUIRED, STATIC  , OTHER, Request, timestamp, action, 0),
    PB_FIELD2(  3, UINT32  , OPTIONAL, STATIC  , OTHER, Request, start, timestamp, 0),
    PB_FIELD2(  4, UINT32  , OPTIONAL, STATIC  , OTHER, Request, count, start, 0),
    PB_FIELD2(  5, BYTES   , OPTIONAL, STATIC  , OTHER, Request, payload, count, 0),
    PB_FIELD2(  6, BOOL    , OPTIONAL, STATIC  , OTHER, Request, repeat, payload, &Request_repeat_default),
    PB_LAST_FIELD
};

const pb_field_t State_fields[3] = {
    PB_FIELD2(  1, UINT32  , REQUIRED, STATIC  , FIRST, State, address, address, 0),
    PB_FIELD2(  2, UINT32  , REQUIRED, STATIC  , OTHER, State, payload, address, 0),
    PB_LAST_FIELD
};

const pb_field_t Sample_fields[3] = {
    PB_FIELD2(  1, UINT64  , REQUIRED, STATIC  , FIRST, Sample, sequence, sequence, 0),
    PB_FIELD2(  2, INT32   , REPEATED, STATIC  , OTHER, Sample, payload, sequence, 0),
    PB_LAST_FIELD
};

const pb_field_t Response_fields[6] = {
    PB_FIELD2(  1, ENUM    , OPTIONAL, STATIC  , FIRST, Response, error, error, &Response_error_default),
    PB_FIELD2(  2, STRING  , OPTIONAL, STATIC  , OTHER, Response, error_msg, error, 0),
    PB_FIELD2(  3, UINT64  , REQUIRED, STATIC  , OTHER, Response, timestamp, error_msg, 0),
    PB_FIELD2(  4, MESSAGE , REPEATED, CALLBACK, OTHER, Response, state, timestamp, &State_fields),
    PB_FIELD2(  5, MESSAGE , REPEATED, CALLBACK, OTHER, Response, sample, state, &Sample_fields),
    PB_LAST_FIELD
};


/* Check that field information fits in pb_field_t */
#if !defined(PB_FIELD_16BIT) && !defined(PB_FIELD_32BIT)
STATIC_ASSERT((pb_membersize(Response, state) < 256 && pb_membersize(Response, sample) < 256), YOU_MUST_DEFINE_PB_FIELD_16BIT_FOR_MESSAGES_Separator_Request_State_Sample_Response)
#endif

#if !defined(PB_FIELD_32BIT)
STATIC_ASSERT((pb_membersize(Response, state) < 65536 && pb_membersize(Response, sample) < 65536), YOU_MUST_DEFINE_PB_FIELD_32BIT_FOR_MESSAGES_Separator_Request_State_Sample_Response)
#endif

