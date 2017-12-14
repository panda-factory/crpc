#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/un.h>

#include "tlv.h"
#include "crpc_srv.h"
#include "define.h"
#include "security.h"


#if DESC("内部函数")
/**
 * 说明：
 * 返回：
 * 备注：
 */

/**
 * 说明：申请crpc实例内存
 * 返回：crpc_inst_t *
 * 备注：返回值外部释放 
 */
static crpc_cli_inst_t *
crpc_cli_inst_new()
{
    int ret = ERROR;
    crpc_cli_inst_t *cli_inst = NULL;

    cli_inst = (crpc_cli_inst_t *)malloc(sizeof(crpc_cli_inst_t));
    CHECK_NULL_RETURN_NULL(cli_inst, "malloc failed.");
    ret = s_memset(cli_inst, sizeof(crpc_cli_inst_t), 0, sizeof(crpc_cli_inst_t));
    if (ERROR == ret) {
        ERROR_LOG("s_memset() failed.");
        s_free(cli_inst);
        return NULL;
    }

    return cli_inst;
}

/**
 * 说明：crpc注册
 * 返回：
 * 备注：
 */
static int
crpc_method_register(crpc_cli_inst_t *cli)
{
    const tlv_t *iter = NULL;
    const uint8_t *crpc_data = NULL;

    CHECK_NULL_RETURN_ERROR(cli, "input param crpc_cli = NULL,");
    CHECK_NULL_RETURN_ERROR(cli->recv_buf, "plugin recv_buf is NULL.");

    crpc_data = buffer_data(cli->recv_buf) + sizeof(crpc_msg_head_t);

    for (iter = (tlv_t *)crpc_data; T_TERMINATOR != iter->type; iter = tlv_next((tlv_t *)iter)) {
        if (T_PLUGIN_NAME == iter->type) {
            cli->name = strdup(iter->value);
            CHECK_NULL_RETURN_ERROR(cli->name, "strdup() failed when register plugin name.");
        }
    }

    cli->flag_register = true;

    return OK;
}

/**
 * 说明：crpc激活
 * 返回：
 * 备注：
 */
static int
crpc_method_activate(crpc_cli_inst_t *cli)
{
    const tlv_t *iter = NULL;
    const uint8_t *crpc_data = NULL;

    CHECK_NULL_RETURN_ERROR(cli, "input param crpc_cli = NULL,");
    CHECK_NULL_RETURN_ERROR(cli->recv_buf, "plugin recv_buf is NULL.");

    cli->flag_activate = true;

    return OK;
}

/**
 * 说明：crpc方法解析函数
 * 返回：
 */
static e_crpc_method
crpc_method_parse(crpc_cli_inst_t *cli)
{
    e_crpc_method method = METHOD_NONE;
    crpc_msg_head_t *head = NULL;

    if (NULL == cli) {
        ERROR_LOG("cannot receive cli = NULL.");
        return METHOD_NONE;
    }

    head = (crpc_msg_head_t *)buffer_data(cli->recv_buf);
    method = head->method;

    return method;
}

/**
 * 说明：crpc方法调度函数
 * 返回：
 */
static int
crpc_method_dispatch(crpc_cli_inst_t *cli)
{
    int ret = ERROR;
    e_crpc_method method = METHOD_NONE;
    CHECK_NULL_RETURN_ERROR(cli, "cannot receive cli = NULL.");

    method = crpc_method_parse(cli);

    switch(method) {
        case METHOD_REGISTER:
            ret = crpc_method_register(cli);
            if (OK == ret) {
                DEBUG_LOG("crpc client [%s] register success!", cli->name);
            }
            else {
                ERROR_LOG("crpc client register failed!");
            }
            break;
        case METHOD_ACTIVATE:
            ret = crpc_method_activate(cli);
            if (OK == ret) {
                DEBUG_LOG("crpc client [%s] activate success!", cli->name);
            }
            else {
                ERROR_LOG("crpc client activate failed!");
            }
            break;
    }

    return OK;
}

/**
 * 说明：服务端接收crpc消息
 * 返回：
 */
static int
crpc_inst_recv_msg(crpc_cli_inst_t *inst)
{
    int ret = ERROR;
    uint8_t recv_buf[BUFFER_SIZE];
    int64_t recv_length = ERROR;

    CHECK_NULL_RETURN_ERROR(inst, "cannot receive cli = NULL.");

    do {
        recv_length = read(inst->sk_fd, recv_buf, BUFFER_SIZE);
        CHECK_ERROR_RETURN_ERROR(recv_length, "read() failed.");

        ret = buffer_append(&inst->recv_buf, recv_buf, recv_length);
    } while (0 != recv_length);

    return OK;
}

/**
 * 说明: 接入客户端sockfd
 * 返回：success: accept_fd; failed: 进程退出
 */
static int
crpc_srv_awaken(crpc_srv_t *srv)
{
    int cli_fd;
    unsigned int cli_addr_len;
    struct sockaddr_un cli_addr;
    struct epoll_event event_register;
    crpc_cli_inst_t *cli = NULL;

    cli = crpc_cli_inst_new();
    cli->id = ++srv->count_inst;

    cli->sk_fd = accept(srv->sk_fd, (struct sockaddr *)&cli_addr, &cli_addr_len);
    if (cli->sk_fd < 0) {
        perror("cannot accept client connect request.");
        return ERROR;
    }

    list_append(&srv->inst_list, &cli->list_node);

    /* epoll注册服务器监听socket*/
    event_register.data.u64 = cli->sk_fd & CRPC_SK_FD_MASK;
    event_register.data.u64 = CRPC_EVENT_CLI & CRPC_EVENT_MASK;
    event_register.data.u64 = (cli->id << 32) & CRPC_INST_ID_MASK;
    event_register.events = EPOLLIN | EPOLLET;
    epoll_ctl(srv->ep_fd, EPOLL_CTL_ADD, cli->sk_fd, &event_register);
    DEBUG_LOG("server accept client.");

    return OK;
}

/**
 * 说明：接收crpc消息
 * 返回：
 * 备注：
 */
static int
crpc_cli_recv_msg(crpc_cli_t *cli)
{
    int ret = ERROR;
    uint8_t recv_buf[BUFFER_SIZE] = {0};
    int64_t recv_length = ERROR;

    CHECK_NULL_RETURN_ERROR(cli, "cannot receive cli = NULL || buf = NULL.");

    do {
        recv_length = read(cli->sk_fd, recv_buf, BUFFER_SIZE);
        CHECK_ERROR_RETURN_ERROR(recv_length, "read() failed.");

        ret = buffer_append(&cli->recv_buf, recv_buf, recv_length);
    } while (0 != recv_length);

    return OK;
}

/**
 * 说明：crpc客户端消息响应
 * 返回：
 * 备注：
 */
static int 
crpc_cli_awaken(crpc_srv_t *srv, const int cli_id)
{
    list_node_t *iter = NULL;
    crpc_cli_inst_t *cli = NULL;

    for (iter = (srv->inst_list).head; NULL != iter; iter = iter->next) {
        cli = CONTAINER_OF(iter, crpc_cli_inst_t, list_node);
        if (cli_id == cli->id) {
            DEBUG_LOG("client awaken.");
            crpc_cli_recv_msg(cli);
            crpc_method_dispatch(cli);
            return OK;
        }
    }

    return ERROR;
}

/**
 * 说明：crpc服务端sk_fd初始化
 * 返回：
 * 备注：
 */
static int
crpc_srv_fd_init(crpc_srv_t *srv)
{
    int ret = ERROR;
    struct sockaddr_un listen_addr;

    srv->sk_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    CHECK_ERROR_RETURN_ERROR(srv->sk_fd, "can not create server socket.");

    listen_addr.sun_family = AF_UNIX;
    strncpy(listen_addr.sun_path, UNIX_DOMAIN, sizeof(listen_addr.sun_path) + 1);
    unlink(UNIX_DOMAIN);

    ret = bind(srv->sk_fd, (struct sockaddr *)&listen_addr, sizeof(listen_addr));
    if (-1 == ret) {
        perror("bind server socket failed.");
        close(srv->sk_fd);
        return ERROR;
    }
    ret = listen(srv->sk_fd, 1);
    if (-1 == ret) {
        perror("listening server socket failed.");
        close(srv->sk_fd);
        unlink(UNIX_DOMAIN);
        return ERROR;
    }

    return OK;
}

/**
 * 说明：crpc服务端epoll初始化
 * 返回：
 * 备注：
 */
static int
crpc_srv_ep_init(crpc_srv_t *srv)
{
    struct epoll_event event_register;

    srv->ep_fd = epoll_create(256);
    CHECK_ERROR_RETURN_ERROR(srv->ep_fd, "epoll_create failed.");

    /* epoll注册服务器监听socket*/
    event_register.data.u64 = srv->sk_fd & CRPC_SK_FD_MASK;
    event_register.data.u64 = CRPC_EVENT_SRV & CRPC_EVENT_MASK;
    event_register.events = EPOLLIN | EPOLLET;
    epoll_ctl(srv->ep_fd, EPOLL_CTL_ADD, srv->sk_fd, &event_register);

    return OK;
}

static void
crpc_srv_ep_run(crpc_srv_t *srv)
{
    int ret = ERROR;
    int i = 0;
    int event_num = 0;
    uint16_t event_type = CRPC_EVENT_NONE >> 48;
    uint16_t cli_id = CRPC_INST_ID_NONE >> 32;
    struct epoll_event event_queue[20];

    for (;;) {
        event_num = epoll_wait(srv->ep_fd, event_queue, 20, 500);
        for (i = 0; i < event_num; i++) {
            event_type = (event_queue[i].data.u64 & CRPC_EVENT_MASK) >> 48;

            if (CRPC_EVENT_SRV == event_type) {
                crpc_srv_awaken(srv);
            } 
            else if (CRPC_EVENT_CLI == event_type) {
                cli_id = (event_queue[i].data.u64 & CRPC_INST_ID_MASK) >> 32;
                crpc_cli_awaken(srv, cli_id);
                cli_id = CRPC_INST_ID_NONE >> 32;
            } 
            else {
                /* 待定*/
            }
            event_type = CRPC_EVENT_NONE >> 48;
        }

    }
    return;
}
#endif //内部函数

#if DESC("外部函数")

#if DESC("服务端API")
/**
 * 说明：申请crpc服务端内存，并初始化
 * 返回：crpc_cli_inst_t *
 * 备注：返回值外部释放
 */
crpc_srv_t *
crpc_srv_new()
{
    int ret = ERROR;
    crpc_srv_t *srv = NULL;
    void *tmp = NULL;

    srv = (crpc_srv_t *)malloc(sizeof(crpc_srv_t));
    CHECK_NULL_RETURN_NULL(srv, "malloc failed.");
    s_memset(srv, sizeof(crpc_srv_t), 0, sizeof(crpc_srv_t));

    return srv;
}

/**
 * 说明：释放crpc服务端实例内存，并将指针赋值NULL
 * 返回：
 * 备注：
 */
void
crpc_srv_free(crpc_srv_t **srv)
{
    if (NULL == srv || NULL == *srv) {
        return;
    }

    s_free((*srv)->name);
    s_free(*srv);

    return;
}

/**
 * 说明：初始化crpc服务端实例
 * 返回：
 * 备注：
 */
int
crpc_srv_init(crpc_srv_t *srv, const char *name)
{
    int ret = ERROR;

    CHECK_2_NULL_RETURN_ERROR(srv, name, "input params cannot be NULL.");

    srv->name = strdup("name");
    CHECK_NULL_RETURN_ERROR(srv->name, "strdup() failed.");

    ret = crpc_srv_fd_init(srv);
    CHECK_ERROR_RETURN_ERROR(ret, "crpc_srv_fd_init() failed.");

    ret = crpc_srv_ep_init(srv);
    CHECK_ERROR_RETURN_ERROR(ret, "crpc_srv_ep_init() failed.");
    
    DEBUG_LOG("crpc server initiate success!.");

    return OK;
}

/**
 * 说明：运行crpc服务端
 * 返回：
 * 备注：
 */
void 
crpc_srv_run(crpc_srv_t *srv)
{
    crpc_srv_ep_run(srv);

    return;
}
#endif //服务端API

#endif //外部函数
