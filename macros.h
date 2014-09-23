
#ifndef _MACROS_H
#define _MACROS_H

#ifndef SIZE_MAX
#define SIZE_MAX ~((size_t)0)
#endif

#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

// stringify macros
#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)

#endif
