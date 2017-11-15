#ifndef _LOG_H_
#define _LOG_H_

#include <stdio.h>
#include <syslog.h>
#include <stdint.h>
#include <pthread.h>
#include <stdbool.h>

#define LOG_IDENTIFIER  ("CRPC")
#define LOG_NONE        (-1)
#define log_syslog      syslog

extern int8_t stderr_level;
extern int8_t syslog_level;

/*
 * 说明: 日志等级
 *
 * LOG_EMERG    紧急
 * LOG_ALERT    报警
 * LOG_CRIT     严重
 * LOG_ERR      错误
 * LOG_WARNING  警告
 * LOG_NOTICE   通知
 * LOG_INFO     信息
 * LOG_DEBUG    调试
 */

/*
 * @说明：将日志等级转换为对应的字符串。
 */
#define LL_TO_STR(LL) \
    ((LOG_EMERG == LL)      ? "EMERG"   : \
     (LOG_ALERT == LL)      ? "ALERT"   : \
     (LOG_CRIT == LL)       ? "CRIT"    : \
     (LOG_ERR == LL)        ? "ERROR"   : \
     (LOG_WARNING == LL)    ? "WARNING" : \
     (LOG_NOTICE == LL)     ? "NOTICE"  : \
     (LOG_INFO == LL)       ? "INFO"    : \
     (LOG_DEBUG == LL)      ? "DEBUG"   : \
                              "NONE")

/**
 * 说明：打印日志信息在syslog.
 */
#define SYSLOG_LOG(LL, fmt, ...) \
    log_syslog(LL, "[%s] [%s] [%lu] (%s: %d) " fmt "\n", LOG_IDENTIFIER, LL_TO_STR(LL), (unsigned long)pthread_self(), __func__, __LINE__, ##__VA_ARGS__)

/**
 * 说明：打印日志信息在stderr.
 */
#define STDERR_LOG(LL, fmt, ...) \
    log_stderr("[%s] [%s] [%lu] (%s: %d) " fmt "\n", LOG_IDENTIFIER, LL_TO_STR(LL), (unsigned long)pthread_self(), __func__, __LINE__, ##__VA_ARGS__)

/**
 * 说明：日志代理器.
 */
#define LOG_PROXY(LL, fmt, ...) \
    do { \
        if (stderr_level >= LL) \
            STDERR_LOG(LL, fmt, ##__VA_ARGS__); \
        if (syslog_level >= LL) \
            SYSLOG_LOG(LL, fmt, ##__VA_ARGS__); \
    } while(0)

/** 
 * 说明：记录EMERG信息. 
 */
#define EMERG_LOG(fmt, ...) LOG_PROXY(LOG_EMERG, fmt, ##__VA_ARGS__) 

/** 
 * 说明：记录ALERT信息. 
 */
#define ALERT_LOG(fmt, ...) LOG_PROXY(LOG_ALERT, fmt, ##__VA_ARGS__) 

/** 
 * 说明：记录CRIT信息.
 */
#define CRIT_LOG(fmt, ...) LOG_PROXY(LOG_CRIT, fmt, ##__VA_ARGS__) 

/** 
 * 说明：记录ERR信息.
 */
#define ERROR_LOG(fmt, ...) LOG_PROXY(LOG_ERR, fmt, ##__VA_ARGS__)

/**
 * 说明：记录WARNING信息.
 */
#define WARNING_LOG(fmt, ...) LOG_PROXY(LOG_WARNING, fmt, ##__VA_ARGS__) 
                                      
/**
 * 说明：记录NOTICE信息.
 */
#define NOTICE_LOG(fmt, ...) LOG_PROXY(LOG_NOTICE, fmt, ##__VA_ARGS__) 
                                      
/** 
 * 说明：记录INFO信息.
 */
#define INFO_LOG(fmt, ...) LOG_PROXY(LOG_INFO, fmt, ##__VA_ARGS__) 
                                          
/**
 * 说明：记录DEBUG信息.
 */
#define DEBUG_LOG(fmt, ...) LOG_PROXY(LOG_DEBUG, fmt, ##__VA_ARGS__) 

void
log_init(bool flag_daemon, int8_t level);

void
log_stderr(const char *fmt, ...);

#endif //_LOG_H_
