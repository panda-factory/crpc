#ifndef _TLV_H_
#define _TLV_H_

#include <stdint.h>

typedef struct _tlv_t {
    uint32_t type;
    uint32_t length;
    uint8_t value[];
} tlv_t;

typedef enum _e_TLV_TYPE {
    TLV_TYPE_NODE,
    TLV_TYPE_HASH_KEY,
    TLV_TYPE_HASH_VALUE,
} e_TLV_TYPE;

int
tlv_init(tlv_t *tlv, const e_TLV_TYPE type, const uint32_t length, const void *value);

uint32_t
tlv_type(tlv_t *tlv);

uint32_t
tlv_length(tlv_t *tlv);

uint8_t *
tlv_value(tlv_t *tlv);

tlv_t *
tlv_next(tlv_t *tlv);

#endif //_TLV_H_
