#include "Logging.h"

void Logging::init(int level, HardwareSerial stream){
    _level = constrain(level,LOG_LEVEL_NOOUTPUT,LOG_LEVEL_VERBOSE);
    _stream = stream;
}

void Logging::error(char* msg, ...){
    if (LOG_LEVEL_ERRORS <= _level) {   
        va_list args;
        va_start(args, msg);
        print(msg,args);
    }
}


void Logging::info(char* msg, ...){
    if (LOG_LEVEL_INFOS <= _level) {
        va_list args;
        va_start(args, msg);
        print(msg,args);
    }
}

void Logging::debug(char* msg, ...){
    if (LOG_LEVEL_DEBUG <= _level) {
        va_list args;
        va_start(args, msg);
        print(msg,args);
    }
}


void Logging::verbose(char* msg, ...){
    if (LOG_LEVEL_VERBOSE <= _level) {
        va_list args;
        va_start(args, msg);
        print(msg,args);
    }
}



 void Logging::print(const char *format, va_list args) {
    //
    // loop through format string
    for (; *format != 0; ++format) {
        if (*format == '%') {
            ++format;
            if (*format == '\0') break;
            if (*format == '%') {
                _stream.print(*format);
                continue;
            }
            if( *format == 's' ) {
				register char *s = (char *)va_arg( args, int );
				_stream.print(s);
				continue;
			}
            if( *format == 'd' || *format == 'i') {
				_stream.print(va_arg( args, int ),DEC);
				continue;
			}
            if( *format == 'x' ) {
				_stream.print(va_arg( args, int ),HEX);
				continue;
			}
            if( *format == 'X' ) {
				_stream.print("0x");
				_stream.print(va_arg( args, int ),HEX);
				continue;
			}
            if( *format == 'b' ) {
				_stream.print(va_arg( args, int ),BIN);
				continue;
			}
            if( *format == 'B' ) {
				_stream.print("0b");
				_stream.print(va_arg( args, int ),BIN);
				continue;
			}
            if( *format == 'l' ) {
				_stream.print(va_arg( args, long ),DEC);
				continue;
			}

            if( *format == 'c' ) {
				_stream.print(va_arg( args, int ));
				continue;
			}
            if( *format == 't' ) {
				if (va_arg( args, int ) == 1) {
					_stream.print("T");
				}
				else {
					_stream.print("F");				
				}
				continue;
			}
            if( *format == 'T' ) {
				if (va_arg( args, int ) == 1) {
					_stream.print("true");
				}
				else {
					_stream.print("false");				
				}
				continue;
			}

        }
        _stream.print(*format);
    }
 }
 
 Logging Log = Logging();

 
 
  




