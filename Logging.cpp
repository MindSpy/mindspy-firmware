#include "Logging.h"

void Logging::init(int level, Print* stream){
    _level = constrain(level,LOG_LEVEL_NOOUTPUT,LOG_LEVEL_VERBOSE);
    _stream = stream;
}

void Logging::error(const String & msg, ...){
    if (LOG_LEVEL_ERRORS <= _level) {   
        va_list args;
        va_start(args, msg);
        print(msg,args);
    }
}


void Logging::info(const String & msg, ...){
    if (LOG_LEVEL_INFOS <= _level) {
        va_list args;
        va_start(args, msg);
        print(msg,args);
    }
}

void Logging::debug(const String & msg, ...){
    if (LOG_LEVEL_DEBUG <= _level) {
        va_list args;
        va_start(args, msg);
        print(msg,args);
    }
}


void Logging::verbose(const String & msg, ...){
    if (LOG_LEVEL_VERBOSE <= _level) {
        va_list args;
        va_start(args, msg);
        print(msg,args);
    }
}



 void Logging::print(const String &format, va_list args) {
    //
    // loop through format string
    for (unsigned int i  = 0; i < format.length() ; ++i ) {
        register char c = format[i];
        if (c == '%') {
            ++i;
            if (i >= format.length()) break;
            c = format[i];
            switch (c) {
              case '%':
                _stream->print(c);
                break;
              case 's': {
  	          register char *s = (char *)va_arg( args, int );
  	          _stream->print(s);
                  break;  
                }
              case 'd':
              case 'i':
	        _stream->print(va_arg( args, int ),DEC);
                break;
              case 'X':
	        _stream->print("0x");
              case 'x':
	        _stream->print(va_arg( args, int ),HEX);
                break;
              case 'B':
                _stream->print("0b");
              case 'b':
	        _stream->print(va_arg( args, int ),BIN);
                break;
              case 'l':
                _stream->print(va_arg( args, long ),DEC);
                break;
              case 'c':
	        _stream->print(va_arg( args, int ));
                break;
              case 't':
	        if (va_arg( args, int ) == 1) {
                  _stream->print("T");
                }  else {
                  _stream->print("F");
                }
                break;
              case 'T':
	        if (va_arg( args, int ) == 1) {
                  _stream->print("true");
                }  else {
                  _stream->print("false");
                }
                break;
            }
        }
        _stream->print(c);
    }
 }
 
 Logging Log = Logging();

 
 
  




