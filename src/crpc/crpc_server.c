#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/un.h>

#include "crpc_server.h"
#include "define.h"

int srv_fd = -1;
int epoll_fd = -1;
list_table_t cli_list;

#if DESC("内部函数")

static crpc_cli_info_t *
crpc_cli_info_new()
{
    crpc_cli_info_t *cli_info = NULL;

    cli_info = (crpc_cli_info_t *)malloc(sizeof(crpc_cli_info_t));
    CHECK_NULL_RETURN_NULL(cli_info, "malloc failed.");
    memset(cli_info, 0, sizeof(crpc_cli_info_t));

    cli_info->send_buf = buffer_new(BUFFER_SIZE);
    cli_info->recv_buf = buffer_new(BUFFER_SIZE);
    bzero(cli_info->send_buf, BUFFER_SIZE);
    bzero(cli_info->recv_buf, BUFFER_SIZE);

    return cli_info;
}

/**
 * 说明: 创建服务器sockfd，并对其监听
 * @return: 失败直接退出
 */
static void
crpc_srv_init()
{
    int ret = ERROR;
    struct sockaddr_un listen_addr;
    
    /*1、创建服务器监听socket*/
    srv_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    CHECK_ERROR_EXIT(srv_fd, "can not create server socket.");

    /* 监听socket绑定本地文件路径，并进行监听*/
    listen_addr.sun_family = AF_UNIX;
    strncpy(listen_addr.sun_path, UNIX_DOMAIN, sizeof(listen_addr.sun_path) + 1);
    unlink(UNIX_DOMAIN);

    ret = bind(srv_fd, (struct sockaddr *)&listen_addr, sizeof(listen_addr));
    if (-1 == ret) {
        perror("bind server socket failed.");
        close(srv_fd);
        exit(1);
    }
    ret = listen(srv_fd, 1);
    if (-1 == ret) {
        perror("listening server socket failed.");
        close(srv_fd);
        unlink(UNIX_DOMAIN);
        exit(1);
    }

    return ;
}

/**
 * 说明: 接入客户端sockfd
 * @return: success: accept_fd; failed: 进程退出
 */
static int
crpc_srv_awaken()
{
    int cli_fd;
    unsigned int cli_addr_len;
    struct sockaddr_un cli_addr;
    struct epoll_event event_register;
    crpc_cli_info_t *cli_info = NULL;

    cli_info = crpc_cli_info_new();

    cli_fd = accept(srv_fd, (struct sockaddr *)&cli_addr, &cli_addr_len);
    if (cli_fd < 0) {
        perror("cannot accept client connect request.");
        return ERROR;
    }

    cli_info->sk_fd = cli_fd;
    list_append(&cli_list, &cli_info->list_node);

    /* epoll注册服务器监听socket*/
    event_register.data.fd = cli_fd;
    event_register.events = EPOLLIN | EPOLLET;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, cli_fd, &event_register);
    DEBUG_LOG("server accept client.");

    return OK;
}

static int 
crpc_cli_awaken(const int sk_fd)
{
    list_node_t *iter = NULL;
    crpc_cli_info_t *cli_info = NULL;

    for (iter = cli_list.head; NULL != iter; iter = iter->next) {
        cli_info = CONTAINER_OF(iter, crpc_cli_info_t, list_node);
        if (sk_fd == cli_info->sk_fd) {
            DEBUG_LOG("client awaken.");
            return OK;
        }
    }

    return ERROR;
}

static void
crpc_epoll_init(void)
{
    struct epoll_event event_register;

    epoll_fd = epoll_create(256);
    CHECK_ERROR_EXIT(epoll_fd, "epoll_create failed.");

    /* epoll注册服务器监听socket*/
    event_register.data.fd = srv_fd;
    event_register.events = EPOLLIN | EPOLLET;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, srv_fd, &event_register);

    return;
}

static void
crpc_epoll_run(void)
{
    int ret = ERROR;
    int i = 0;
    int event_num = 0;
    struct epoll_event event_queue[20];

    for (;;) {
        event_num = epoll_wait(epoll_fd, event_queue, 20, 500);
        for (i = 0; i < event_num; i++) {
            if (event_queue[i].data.fd == srv_fd) {
                crpc_srv_awaken();
            } 
            else if (event_queue[i].data.fd > 0) {
                crpc_cli_awaken(event_queue[i].data.fd);
            } 
            else {
                /* 待定*/
            }
        }

    }
    return;
}
#endif //内部函数

void
crpc_init()
{
    crpc_srv_init();
    crpc_epoll_init();
    
    INFO_LOG("crpc initiate success!.");

    return;
}

void crpc_run()
{
    crpc_epoll_run();

    return;
}
