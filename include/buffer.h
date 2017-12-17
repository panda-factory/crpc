#ifndef _BUFFER_H
#define _BUFFER_H

#include <stdint.h>

typedef struct _buffer_t {
    uint32_t    total;
    uint32_t    used;
    uint8_t     data;
} buffer_t;

buffer_t *
buffer_new(const size_t size)__attribute__ ((warn_unused_result));

extern void
buffer_free(buffer_t *buf);

extern int 
buffer_flush(buffer_t *buf);

extern int
buffer_clear(buffer_t *buf, size_t start, size_t length);

extern int
buffer_append(buffer_t **buf, const void *data_src, const size_t src_size);

extern int 
buffer_expand(buffer_t **buf, const size_t expand_size);

extern int
buffer_reset(buffer_t *buf);

extern void *
buffer_data(buffer_t *buf);

extern uint32_t
buffer_used(const buffer_t *buf);

extern uint32_t
buffer_unused(const buffer_t *buf);

extern uint32_t
buffer_total(const buffer_t *buf);

#endif //_BUFFER_H
