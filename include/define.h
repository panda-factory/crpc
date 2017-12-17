#ifndef _DEFINE_H
#define _DEFINE_H

#include "log.h"
#include "error.h"

#define DESC(x) (1)

#if DESC("错误检查")
#define CHECK_NULL_RETURN_ZERO(ptr, fmt, ...) \
    do { \
        if (NULL == ptr) \
        { \
            ERROR_LOG(fmt, ##__VA_ARGS__); \
            return 0; \
        } \
    } while (0)

#define CHECK_NULL_RETURN_VOID(ptr, fmt, ...) \
    do { \
        if (NULL == ptr) { \
            ERROR_LOG(fmt, ##__VA_ARGS__); \
            return; \
        } \
    } while (0)

#define CHECK_NULL_RETURN_NULL(ptr, fmt, ...) \
    do { \
        if (NULL == ptr) { \
            ERROR_LOG(fmt, ##__VA_ARGS__); \
            return NULL; \
        } \
    } while(0) 

#define CHECK_NULL_RETURN_ERROR(ptr, fmt, ...) \
    do { \
        if (NULL == ptr) { \
            ERROR_LOG(fmt, ##__VA_ARGS__); \
            return ERROR; \
        } \
    } while(0) 

#define CHECK_NULL_EXIT(ptr, fmt, ...) \
    do { \
        if (NULL == ptr) { \
            ERROR_LOG(fmt, ##__VA_ARGS__); \
            exit(1); \
        } \
    } while (0)

#define CHECK_2_NULL_RETURN_NULL(ptr1, ptr2, fmt, ...) \
    do { \
        if (NULL == ptr1 || NULL == ptr2) { \
            ERROR_LOG(fmt, ##__VA_ARGS__); \
            return NULL; \
        } \
    } while(0) 

#define CHECK_2_NULL_RETURN_ERROR(ptr1, ptr2, fmt, ...) \
    do { \
        if (NULL == ptr1 || NULL == ptr2) { \
            ERROR_LOG(fmt, ##__VA_ARGS__); \
            return ERROR; \
        } \
    } while(0) 

#define CHECK_ERROR_RETURN_NULL(ret, fmt, ...) \
    do { \
        if (ERROR == ret) { \
            ERROR_LOG(fmt, ##__VA_ARGS__); \
            return NULL; \
        } \
    } while (0)

#define CHECK_ERROR_RETURN_ERROR(ret, fmt, ...) \
    do { \
        if (ERROR == ret) { \
            ERROR_LOG(fmt, ##__VA_ARGS__); \
            return ERROR; \
        } \
    } while (0)

#define CHECK_ERROR_EXIT(ret, fmt, ...) \
    do { \
        if (ERROR == ret) { \
            ERROR_LOG(fmt, ##__VA_ARGS__); \
            exit(1); \
        } \
    } while (0);
#endif //错误检查

#define CHECK_OK_RETURN_RET(ret, fmt, ...) \
    do { \
        if (OK != ret) { \
            ERROR_LOG(fmt, ##__VA_ARGS__); \
            return ret; \
        } \
    } while (0)

#define CHECK_OK_RETURN_NULL(ret, fmt, ...) \
    do { \
        if (OK != ret) { \
            ERROR_LOG(fmt, ##__VA_ARGS__); \
            return NULL; \
        } \
    } while (0)

#endif //DEFINE_H
