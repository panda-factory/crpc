#include <getopt.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <error.h>
#include <errno.h>
#include <fcntl.h>

#include "buffer.h"
#include "define.h"
#include "hash.h"
#include "error.h"
#include "crpc_srv.h"

#define WORK_PATH           ("/usr/local/etc/crpc")
#define DAEMON_PID_FILE     ("/usr/local/etc/crpc/crpc.pid")
#define SOCKET_HASH_KEY     ("socket"
#define BUFFER_SIZE         (1024)
#define BUNDLES_NUM         (1)
#define HASH_NAME_CTX       ("context")
#define HASH_KEY_EPOLL      ("epoll")
#define HASH_KEY_SRV        ("server")
#define DEFAULT_LOG_LEVEL   (LOG_DEBUG)

hash_table_t *g_ctx_ht;


static char const short_options[] = 
    "dl:h";

static struct option const long_options[] = {
    {"daemon", no_argument, NULL, 'd'},
    {"log", required_argument, NULL, 'l'},
    {"help", no_argument, NULL, 'h'},
    {NULL, 0, NULL, 0}
};

#if DESC("内部函数")
static void
print_help()
{
    printf("Usage:\n");
    printf("\t[-h] [-d] [-l <level>]\n\n");
    printf("Options:\n");
    printf("\t-h\t\tPrints usage help.\n");
    printf("\t-v\t\tPrints version.\n");
    printf("\t-d\t\tDebug mode - daemon will run in the foreground and print logs to stderr instead of syslog.\n");
    printf("\t-l <level>\tSets verbosity level of logging:\n");
    printf("\t\t0 = LOG_EMERG\n");
    printf("\t\t1 = LOG_ALERT\n");
    printf("\t\t2 = LOG_CRIT\n");
    printf("\t\t3 = LOG_ERR [default]\n");
    printf("\t\t4 = LOG_WARNING\n");
    printf("\t\t5 = LOG_NOTICE\n");
    printf("\t\t6 = LOG_INFO\n");
    printf("\t\t7 = LOG_DEBUG\n");

    return;
}

static void
daemon_check_single_instance()
{
    int ret = ERROR;
    int pid_file_fd = -1;
    char str_pid[128];

    /*1、只读方式打开PID文件，不存在则创建.*/
    pid_file_fd = open(DAEMON_PID_FILE, O_RDWR | O_CREAT, 0640);
    CHECK_ERROR_EXIT(pid_file_fd, "Unable to open daemon PID file: [%s].", DAEMON_PID_FILE);

    /*2、PID文件加锁.*/
    ret = lockf(pid_file_fd, F_TLOCK, 0);
    if (ERROR == ret) {
        if (EACCES == errno || EAGAIN == errno) {
            ERROR_LOG("Another instance of sysrepo daemon is running, unable to start.");
        }
        else {
            ERROR_LOG("Unable to lock sysrepo PID file: [%s].", DAEMON_PID_FILE);
        }
        exit(EXIT_FAILURE);
    }

    /*3、进程ID写入PID文件.*/
    ret = snprintf(str_pid, 128, "%d\n", getpid());
    str_pid[ret] = '\0';
    ret = write(pid_file_fd, str_pid, strlen(str_pid));
    CHECK_ERROR_EXIT(ret, "Unable to write into sysrepo PID file: [%s].", DAEMON_PID_FILE);

    /*4、释放资源.*/
    return;
}

static void 
daemon_run()
{
    int ret = ERROR;
    pid_t pid = -1;

    pid = fork();
    CHECK_ERROR_EXIT(pid, "fork() error.");

    daemon_check_single_instance();

    /*1.父进程退出，保留子进程。*/
    if(pid > 0 ) {
        exit(EXIT_SUCCESS);
    }
    
    /*2.成立新会话组*/
    ret = setsid();
    CHECK_ERROR_EXIT(ret, "setsid() error.");
    
    INFO_LOG("daemon is runing");
    /*3.改变工作目录路径*/
    ret = chdir(WORK_PATH);
    if (ERROR == ret) {
        ERROR_LOG("change directory failed.");
        return;
    }
    
    /*4.关闭残留文件描述符*/
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    /*5.重设文件创建掩膜*/
    umask(0);
    return;
}
#endif //内部函数

int main(int argc, char **argv)
{
    int ret = ERROR;
    char optc;
    bool flag_daemon = false;
    int8_t log_level = DEFAULT_LOG_LEVEL;
    int opt_syslog_level = LOG_NONE;
    int opt_stderr_level = LOG_NONE;
    crpc_srv_t *crpc_srv = NULL;


    while (ERROR != (optc = getopt_long(argc, argv, short_options, long_options, NULL))) {
        switch (optc) {
            case 'd':
                flag_daemon = true;
                break;
            case 'l':
                log_level = atoi(optarg);
                break;
            default:
                print_help();
                return 0;
        }
    }

    if (true == flag_daemon) {
        daemon_run();
    }

    log_init(flag_daemon, log_level);
    crpc_srv = crpc_srv_new();
    ret = crpc_srv_init(crpc_srv, "test");
    if (ERROR == ret) {
        ERROR_LOG("Initiate crpc server failed.");
        exit(1);
    }

    crpc_srv_run(crpc_srv);

    return 0;
}
