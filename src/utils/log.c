#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include "log.h"
#include "define.h"

int8_t stderr_level = LOG_DEBUG;
int8_t syslog_level = LOG_NONE;

#if DESC("内部函数")
static void 
set_stderr_log(const int8_t level)
{
    if (level > LOG_DEBUG) {
        fprintf(stderr, "[stderr_level = %d] is not support, set to LOG_NONE.", level);
        stderr_level = LOG_NONE;
        return;
    }

    stderr_level = level;
    return;
}

static void 
set_syslog_log(const int8_t level)
{
    if (level > LOG_DEBUG) {
        fprintf(stderr, "[syslog_level = %d] is not support, set to LOG_NONE.", level);
        syslog_level = LOG_NONE;
        return;
    }

    syslog_level = level;
    return;
}
#endif //内部函数

/**
 * 说明：log初始化
 */
void 
log_init(bool flag_daemon, int8_t level)
{
    if (true == flag_daemon) {
        set_syslog_log(level);
    }
    else {
        set_stderr_log(level);
    }

    INFO_LOG("log initiate success.");
    return;
}

/**
 * 说明：将log信息记录到stderr
 */
void
log_stderr(const char *fmt, ...)
{
    va_list ap;
    time_t current;
    struct tm *time_info;

    time(&current);
    time_info = localtime(&current);

    va_start(ap, fmt);
    fprintf(stderr, "%s", asctime(time_info));
    (void)vfprintf(stderr, fmt, ap);
    va_end(ap);

    (void)fflush(stderr);

    return;
}

