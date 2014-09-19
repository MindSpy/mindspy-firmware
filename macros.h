

#ifndef _MACROS_H
#define _MACROS_H

#ifndef SIZE_MAX
#define SIZE_MAX ~((size_t)0)
#endif

#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

#endif