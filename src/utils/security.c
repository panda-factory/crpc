#include <string.h>

#include "log.h"
#include "define"
#include "security.h"
#include "error.h"


int
s_memset(void *dst, size_t max_dst, int ch, size_t count)
{
    if (NULL == dst) {
        ERROR_LOG("cannot receive dst = NULL.");
        return ERROR;
    }
    else if (max_dst < count) {
        ERROR_LOG("cannot receive max_dst < count.");
        return ERROR;
    }
    else {
        ;
    }

    memset(dst, ch, count);

    return OK;
}

int
s_memcpy(void *dst, const size_t max_dst, const void *src, const size_t count)
{
    if (NULL == dst || NULL == src) {
        ERROR_LOG("cannot receive dst = NULL || src = NULL.");
        return ERROR;
    }
    else if (max_dst < count) {
        ERROR_LOG("cannot receive max_dst < count.");
        return ERROR;
    }
    else {
        ;
    }

    memcpy(dst, src, count);

    return OK;
}

void *
s_malloc_zero(size_t size)
{
	void *ptrmem = NULL;

	ptrmem = malloc(size);
	CHECK_NULL_RETURN_NULL(ptrmem, "Alloc memory failed.");

	(void)s_memset(ptrmem, size, 0, size);

	return ptrmem;
}

