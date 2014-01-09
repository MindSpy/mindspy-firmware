#ifndef PB_COMMONS_H
#define PB_COMMONS_H

#include <Arduino.h>
#include <pb.h>
#include "regs_pb.h"

typedef struct _response_handler_t {
    Request_Action action;
    bool (*handler)(Request*, Response*);
} response_handler_t;

class StreamWrapper {
  protected:
    static Stream* _serial;
    static response_handler_t* _handlers;
    static uint8_t _handler_count;
    static pb_istream_t _input;
    static pb_ostream_t _output;
    
    static bool write_callback(pb_ostream_t *, const uint8_t *, size_t );
    static bool read_callback(pb_istream_t *, uint8_t *, size_t );
  public:
    /*!
     * Returns NanoPB input stream instance.
     * \param void
     * \return void 
     */ 
    inline static pb_istream_t* input(void) {return &_input;}
    
    /*!
     * Returns NanoPB output stream instance.
     * \param void
     * \return void 
     */ 
    inline static pb_ostream_t* output(void) {return &_output;}
    
    /*!
     * Initializes StreamWrapper with stream instance and response handlers.
     * \param serial stream instance
     * \param handler array of response handlers
     * \param handler_count handler count
     * \return void
     */
    static void init(Stream*, response_handler_t*, uint8_t);
    
    /*!
     * Receives request message and returns response. Intercepts message 
     * from the input stream, performs an action based on the request then 
     * generates and submits the response message to the output stream.
     * \param void
     * \return void
     */
    static void handle(void);
    
    /*!
     *
     */
    static void error(Response*,const Response_ErrNo,const char[]);
};



#endif