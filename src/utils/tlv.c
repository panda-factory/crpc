#include <string.h>

#include "tlv.h"
#include "define.h"

int
tlv_init(tlv_t *tlv, const e_TLV_TYPE type, const uint32_t length, const void *value)
{
    CHECK_NULL_RETURN_ERROR(tlv, "cannot receive tlv=(null).");

    tlv->type = type;
    tlv->length = length;
    memcpy(tlv_value(tlv), value, length);

    return OK;
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
