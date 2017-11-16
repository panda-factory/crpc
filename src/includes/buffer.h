#ifndef _BUFFER_H
#define _BUFFER_H

#include <stdint.h>

#define buffer_total(buf)       ((buf)->total)
#define buffer_used(buf)        ((buf)->used)
#define buffer_unused(buf)    ((buf)->total - (buf)->used)

typedef struct _buffer_t {
    size_t    total;
    size_t    used;
    uint8_t     data;
} buffer_t;

buffer_t *
buffer_new(const size_t size);

extern void
buffer_free(buffer_t *buf);

extern int 
buffer_flush(buffer_t *buf);

extern int
buffer_clear(buffer_t *buf, size_t start, size_t length);

extern int
buffer_append(buffer_t *buf, const void *data_src, const size_t src_size);

extern int 
buffer_expand(buffer_t **buf, const size_t expand_size);

extern int
buffer_reset(buffer_t *buf);

extern uint8_t *
buffer_data(buffer_t *buf);
#endif //_BUFFER_H
