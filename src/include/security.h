#ifndef _XSTDLIB_H_
#define _XSTDLIB_H_

#include <stdlib.h>

#define s_free(ptr_v) \
    do { \
        if (NULL == ptr_v) { \
            break; \
        } \
        free(ptr_v); \
        ptr_v = NULL; \
    } while (0)

extern int
s_memset(void *dst, const size_t max_dst, const int ch, const size_t count);

extern int
s_memcpy(void *dst, const size_t max_dst, const void *src, const size_t count);

#endif  //_XSTDLIB_H_
