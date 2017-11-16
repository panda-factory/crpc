#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/un.h>

#include "tlv.h"
#include "crpc_server.h"
#include "define.h"

int srv_fd = -1;
int epoll_fd = -1;
list_table_t plugin_list;

#if DESC("内部函数")
/**
 * 说明：
 * 返回：
 * 备注：
 */

/**
 * 说明：crpc注册
 * 返回：
 * 备注：
 */
static int
crpc_method_register(crpc_plugin_inst_t *plugin_inst)
{
    const tlv_t *iter = NULL;
    const uint8_t *crpc_data = NULL;

    CHECK_NULL_RETURN_ERROR(plugin_inst, "input param plugin_inst = NULL,");
    CHECK_NULL_RETURN_ERROR(plugin_inst->recv_buf, "plugin recv_buf is NULL.");

    crpc_data = buffer_data(plugin_inst->recv_buf) + sizeof(crpc_msg_head_t);

    for (iter = (tlv_t *)crpc_data; T_TERMINATOR != iter->type; iter = tlv_next((tlv_t *)iter)) {
        if (T_PLUGIN_NAME == iter->type) {
            plugin_inst->name = strdup(iter->value);
            CHECK_NULL_RETURN_ERROR(plugin_inst->name, "strdup() failed when register plugin name.");
        }
    }

    plugin_inst->flag_register = true;

    return OK;
}

/**
 * 说明：申请crpc客户端内存，并初始化
 * 返回：crpc_plugin_inst_t *
 * 备注：返回值外部释放
 */
static crpc_plugin_inst_t *
crpc_plugin_inst_new()
{
    crpc_plugin_inst_t *plugin_inst = NULL;

    plugin_inst = (crpc_plugin_inst_t *)malloc(sizeof(crpc_plugin_inst_t));
    CHECK_NULL_RETURN_NULL(plugin_inst, "malloc failed.");
    memset(plugin_inst, 0, sizeof(crpc_plugin_inst_t));

    plugin_inst->send_buf = buffer_new(BUFFER_SIZE);
    plugin_inst->recv_buf = buffer_new(BUFFER_SIZE);
    bzero(plugin_inst->send_buf, BUFFER_SIZE);
    bzero(plugin_inst->recv_buf, BUFFER_SIZE);

    return plugin_inst;
}

/**
 * 说明: 创建服务器sockfd，并对其监听
 * 返回：失败直接退出
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
 * 返回：success: accept_fd; failed: 进程退出
 */
static int
crpc_srv_awaken()
{
    int cli_fd;
    unsigned int cli_addr_len;
    struct sockaddr_un cli_addr;
    struct epoll_event event_register;
    crpc_plugin_inst_t *plugin_inst = NULL;

    plugin_inst = crpc_plugin_inst_new();

    cli_fd = accept(srv_fd, (struct sockaddr *)&cli_addr, &cli_addr_len);
    if (cli_fd < 0) {
        perror("cannot accept client connect request.");
        return ERROR;
    }

    plugin_inst->sk_fd = cli_fd;
    list_append(&plugin_list, &plugin_inst->list_node);

    /* epoll注册服务器监听socket*/
    event_register.data.fd = cli_fd;
    event_register.events = EPOLLIN | EPOLLET;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, cli_fd, &event_register);
    DEBUG_LOG("server accept client.");

    return OK;
}

/**
 * 说明：crpc方法调度函数
 * 返回：
 */
static int
crpc_method_dispatch(crpc_plugin_inst_t *plugin_inst)
{
    return OK;
}

/**
 * 说明：客户端接收crpc消息
 * 返回：
 */
static int
crpc_plugin_recv_msg(crpc_plugin_inst_t *plugin_inst)
{
    char recv_buf[BUFFER_SIZE];

    read(plugin_inst->sk_fd, recv_buf, BUFFER_SIZE);

    DEBUG_LOG("plugin recv msg: %s", recv_buf);
    return OK;
}

/**
 * 说明：crpc客户端消息响应
 * 返回：
 * 备注：
 */
static int 
crpc_plugin_awaken(const int sk_fd)
{
    list_node_t *iter = NULL;
    crpc_plugin_inst_t *plugin_inst = NULL;

    for (iter = plugin_list.head; NULL != iter; iter = iter->next) {
        plugin_inst = CONTAINER_OF(iter, crpc_plugin_inst_t, list_node);
        if (sk_fd == plugin_inst->sk_fd) {
            DEBUG_LOG("client awaken.");
            crpc_plugin_recv_msg(plugin_inst);
            crpc_method_dispatch(plugin_inst);
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
                crpc_plugin_awaken(event_queue[i].data.fd);
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
