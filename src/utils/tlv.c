#include <string.h>

#include "tlv.h"
#include "define.h"
#include "security.h"

int
tlv_init(tlv_t *tlv, const uint32_t type, const uint32_t length, const void *value)
{
    int ret = ERROR;
    CHECK_NULL_RETURN_ERROR(tlv, "cannot receive tlv=(null).");

    tlv->type = type;
    tlv->length = length;
    if (NULL != value) {
        ret = s_memcpy(tlv_value(tlv), length, value, length);
        CHECK_ERROR_RETURN_ERROR(ret, "s_memcpy() failed.");
    }

    return OK;
}

tlv_t *
tlv_new(const uint32_t type, const uint32_t length, const void *value)
{
    int ret = ERROR;
    uint32_t memory_size = 0;
    tlv_t *tlv = NULL;

    memory_size = sizeof(tlv_t) + length;

    tlv = (tlv_t *)malloc(memory_size);
    CHECK_NULL_RETURN_NULL(tlv, "malloc() failed.");

    ret = tlv_init(tlv, type, length, value);
    if (ERROR == ret) {
        ERROR_LOG("tlv_init() failed.");
        s_free(tlv);
        return NULL;
    }

    return tlv;
}

int
tlv_renew(tlv_t **tlv, const uint32_t type, const uint32_t length, const void *value)
{
    CHECK_NULL_RETURN_ERROR(*tlv, "cannot receive *tlv = NULL.");

    tlv_free(tlv);
    *tlv = tlv_new(type, length, value);

    return OK;
}

extern void
tlv_free(tlv_t **tlv)
{
    s_free(*tlv);

    return;
}

uint32_t
tlv_type(tlv_t *tlv)
{
    CHECK_NULL_RETURN_ZERO(tlv, "cannot receive tlv = (null).");

    return tlv->type;
}

uint32_t
tlv_length(tlv_t *tlv)
{
    CHECK_NULL_RETURN_ZERO(tlv, "cannot receive tlv = (null).");

    return tlv->length;
}

uint8_t *
tlv_value(tlv_t *tlv)
{
    CHECK_NULL_RETURN_ZERO(tlv, "cannot receive tlv = (null).");

    return (uint8_t *)&tlv->value;
}

tlv_t *
tlv_next(tlv_t *tlv)
{
    tlv_t *next;
    CHECK_NULL_RETURN_NULL(tlv, "cannot receive tlv=(null).");

    next = (tlv_t *)((uint8_t *)tlv + sizeof(tlv_t) + tlv_length(tlv));
    return next; 
}
