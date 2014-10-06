#ifndef COMPAT_HPP
#define COMPAT_HPP

// this is for compatibility with unit tests when wiring is not available
#ifdef _UNITTEST

#include <stdint.h>
#include <stdlib.h>

/*!
 * \brief Wait for certain interval.
 * \param us
 */
void delayMicroseconds(unsigned int us);

/*!
 * \brief millis
 * \return number of miliseconds from start of epoch
 */
unsigned long millis();

/*!
 * \brief micros
 * \return number of microseconds from start of epoch
 */
unsigned long micros();

/*!
 * \brief The Stream class
 */
class Stream {
public:
    virtual ~Stream() {}
    virtual int available() = 0;
    virtual size_t readBytes( char *buffer, size_t length) = 0;
    virtual size_t write(const uint8_t *buffer, size_t size) = 0;
};

#endif

#endif // COMPAT_H
