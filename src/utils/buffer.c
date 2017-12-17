#include <stdlib.h>
#include <string.h>

#include "buffer.h"
#include "define.h"
#include "error.h"

/**
 * @brief: new buffer memory
 * @param:
 *      size memory size for buffer.
 *
 * @return:
 *      buffer_t * new buffer memory address.
 */
buffer_t *
buffer_new(const size_t size)
{
    buffer_t *buf = NULL;
    size_t memory_size;

    memory_size = sizeof(buffer_t) + size;
    buf = (buffer_t *)malloc(memory_size);
    CHECK_NULL_RETURN_NULL(buf, "malloc buffer failure.");

    buf->total = size;
    buf->used = 0;

    return buf;
}

/**
 * @brief: free buffer memory
 * @param:
 *      buf wanna to free buffer address.
 *
 * @return:
 */
void
buffer_free(buffer_t *buf)
{
    CHECK_NULL_RETURN_VOID(buf, "input param buf == NULL.");
    free(buf);
}

/**
 * @brief: flush buffer unused-memory to 0.
 * @param:
 *      buf buffer will to flush.
 *
 * @return:
 *      OK for success, ERROR-CODE for others.
 */
int 
buffer_flush(buffer_t *buf)
{
    uint8_t *start_addr;
    size_t unused_size;

    CHECK_NULL_RETURN_ERROR(buf, "input params buf == NULL.");

    start_addr = buffer_data(buf) + buffer_used(buf);
    unused_size = buffer_unused(buf);

    memset(start_addr, 0, unused_size);

    return OK;
}

/**
 * @brief: clear buffer data from position start to start+length
 * @param:
 *      buf     buffer address.
 *      start   start position to clear
 *      length  length to clear
 *
 * @return:
 *      OK for success, ERROR-CODE for others.
 */
int 
buffer_clear(buffer_t *buf, size_t start, size_t length)
{
    uint8_t *start_addr;

    CHECK_NULL_RETURN_ERROR(buf, "input param buf == NULL.");
    if (start < 0 || length < 0) {
        ERROR_LOG("start || length can not be <0.");
        return ERROR;
    }

    if (start >= buffer_used(buf)) {
        /* buffer[(begin)   (used)  (start)     (end)]*/
        buffer_flush(buf);
    } else if (buffer_used(buf) - start <= length) {
        /* buffer[(begin)   (start) (used)  (length)    (end)]*/
        buf->used = start;
        buffer_flush(buf);
    } else {
        /* buffer[(begin)   (start) (length)    (used)  (end)]*/
        memmove(buffer_data(buf) + start, \
                buffer_data(buf) + start + length, \
                buffer_used(buf) - start - length);
        buf->used = buffer_used(buf) - length;
        buffer_flush(buf);
    }

    return OK;
}

/**
 * @brief: append data to buffer tail.
 * @param:
 *      buf:         
 *          buffer will to flush.
 *      data_src    
 *          data address to append.
 *      data_size   
 *          data size.
 *
 * @return:
 *      OK for success, ERROR-CODE for others.
 */
int
buffer_append(buffer_t **buf, const void *data_src, const size_t data_size)
{
    uint8_t *data_dst = NULL;
    size_t free_size;

    CHECK_2_NULL_RETURN_ERROR(*buf, data_src, "input params buf || data == NULL.");
    free_size = buffer_unused(*buf);
    if (free_size < data_size) {
        ERROR_LOG("the free size in buffer is not enough to accept source data.");
        return ERROR;
    }

    data_dst = buffer_data(*buf) + buffer_used(*buf);
    memcpy(data_dst, data_src, data_size);
    (*buf)->used += data_size;

    return OK;
}

int 
buffer_expand(buffer_t **buf, const size_t expand_size)
{
    size_t new_size;

    CHECK_NULL_RETURN_ERROR(*buf, "input params buf == NULL.");
    
    new_size = buffer_total(*buf) + expand_size;
    *buf = (buffer_t *)realloc(*buf, new_size);
    CHECK_NULL_RETURN_ERROR(*buf, "expand buffer failed.");
    memset(buffer_data(*buf) + buffer_used(*buf), 0, expand_size);

    return OK;
}

int 
buffer_reset(buffer_t *buf)
{
    CHECK_NULL_RETURN_ERROR(buf, "input param buf == NULL.");

    memset(buffer_data(buf), 0, buffer_total(buf));

    return OK;
}

void *
buffer_data(buffer_t *buf)
{
    CHECK_NULL_RETURN_NULL(buf, "input params buf == NULL.");

    return &buf->data;
}

uint32_t
buffer_used(const buffer_t *buf)
{
    CHECK_NULL_RETURN_ERROR(buf, "cannot receive buf = NULL.");

    return buf->used;
}

uint32_t
buffer_unused(const buffer_t *buf)
{
    CHECK_NULL_RETURN_ERROR(buf, "cannot receive buf = NULL.");

    return buf->total - buf->used;
}

uint32_t
buffer_total(const buffer_t *buf)
{
    CHECK_NULL_RETURN_ERROR(buf, "cannot receive buf = NULL.");

    return buf->total;
}

