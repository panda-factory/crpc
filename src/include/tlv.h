#ifndef _TLV_H_
#define _TLV_H_

#include <stdint.h>

#define TLV_CHECK_NULL_RETURN_ERROR(tlv, fmt, ...) \
    do { \
        if (NULL == tlv) { \
            ERROR_LOG(fmt, ##__VA_ARGS__); \
            tlv_free(&(tlv)); \
            return ERROR; \
        } \
    } while (0) 

#define TLV_CHECK_ERROR_RETURN_ERROR(ret, fmt, ...) \
    do { \
        if (ERROR == ret) { \
            ERROR_LOG(fmt, ##__VA_ARGS__); \
            tlv_free(&(tlv)); \
            return ERROR; \
        } \
    } while (0)

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

typedef enum _e_CRPC_TYPE{
    T_NONE,
    T_PLUGIN_NAME,
    T_TERMINATOR,
} e_CRPC_TYPE;

extern int
tlv_init(tlv_t *tlv, const uint32_t type, const uint32_t length, const void *value);

extern tlv_t *
tlv_new(const uint32_t type, const uint32_t length, const void *value);

extern int
tlv_renew(tlv_t **tlv, const uint32_t type, const uint32_t length, const void *value);

extern void
tlv_free(tlv_t **tlv);

extern uint32_t
tlv_type(tlv_t *tlv);

extern uint32_t
tlv_length(tlv_t *tlv);

extern uint8_t *
tlv_value(tlv_t *tlv);

extern tlv_t *
tlv_next(tlv_t *tlv);

#endif //_TLV_H_
