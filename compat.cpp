#include "compat.hpp"

#ifdef _UNITTEST

#include <unistd.h>
#include <sys/time.h>

unsigned long millis()
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return tp.tv_sec * 1000 + tp.tv_usec / 1000;
}

unsigned long micros()
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return tp.tv_sec * 1000000 + tp.tv_usec;
}

void delayMicroseconds(unsigned int us)
{
    usleep(us);
}

#endif
