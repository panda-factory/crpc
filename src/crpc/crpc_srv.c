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
#include "crpc_protobuf.pb-c.h"

CrpcCallback g_crpc_callback[CRPC_CALLBACK_BUTT] = {0};


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
    crpc_cli_inst_t *cli = NULL;

    cli = (crpc_cli_inst_t *)malloc(sizeof(crpc_cli_inst_t));
    CHECK_NULL_RETURN_NULL(cli, "malloc failed.");
    
    ret = s_memset(cli, sizeof(crpc_cli_inst_t), 0, sizeof(crpc_cli_inst_t));
    CHECK_OK_RETURN_NULL(ret, "s_memset() failed.");

    cli->recv_buf = buffer_new(BUFFER_SIZE);
    CHECK_NULL_RETURN_NULL(cli->recv_buf, "buffer new for receive buffer of client instance failed.");

    cli->send_buf = buffer_new(BUFFER_SIZE);
    CHECK_NULL_RETURN_NULL(cli->send_buf, "buffer new for send buffer of client instance failed.");

    return cli;
}

#if DESC("method 函数")
/**
 * 说明：crpc hello world函数，用于测试
 * 返回：
 * 备注：
 */
static int 
crpc_method_helloworld()
{
	DEBUG_LOG("Hello World!");
}

/**
 * 说明：crpc method 安装
 * 返回：
 * 备注：
 */
static int 
crpc_callback_install(    e_CrpcCallback callback_id,   int (*pfunc)(void *, ...))
{
	if (CRPC_CALLBACK_BUTT <= callback_id) {
		ERROR_LOG("CRpc method name is wrong. [%u]", callback_id);
		return ERROR;
	}
	g_crpc_callback[callback_id] = pfunc;

	return OK;
}


#endif

#if DESC("operate 函数")
/**
 * 说明：crpc注册
 * 返回：
 * 备注：
 */
static int
crpc_operate_install(crpc_cli_inst_t *cli, CrpcMessageHead *ptr_crpc_msg)
{
	CrpcMessageAck crpc_msg_ack;
    CHECK_NULL_RETURN_ERROR(cli, "input param crpc_cli = NULL,");
    CHECK_NULL_RETURN_ERROR(ptr_crpc_msg, "input param crpc_cli = NULL,");

    cli->name = strdup(ptr_crpc_msg->name);
    CHECK_NULL_RETURN_ERROR(cli->name, "strdup() failed when install plugin name.");

    cli->flag_install = true;
	
    return OK;
}

/**
 * 说明：crpc激活
 * 返回：
 * 备注：
 */
static int
crpc_operate_activate(crpc_cli_inst_t *cli, CrpcMessageHead *ptr_crpc_msg)
{
	CrpcMessageAck crpc_msg_ack;

    CHECK_NULL_RETURN_ERROR(cli, "input param crpc_cli = NULL,");

    cli->flag_activate = true;
	
    return OK;
}

/**
 * 说明：crpc注册
 * 返回：
 * 备注：
 */
static int
crpc_operate_callback(crpc_cli_inst_t *cli, CrpcMessageHead *ptr_crpc_msg)
{
	CrpcCallbackRequest *ptr_crpc_cb_reg;

	ptr_crpc_cb_reg = crpc_callback_request__unpack(NULL, ptr_crpc_msg->content.len, ptr_crpc_msg->content.data);

    //cli->callback = g_crpc_callback;
	
    DEBUG_LOG("crpc callback: [%d]", ptr_crpc_cb_reg->callback_id);

    return OK;
}


/**
 * 说明：crpc操作解析函数
 * 返回：
 */
static e_CrpcMsgType
crpc_get_msg_type(CrpcMessageHead *ptr_crpc_msg)
{
    if (NULL == ptr_crpc_msg) {
        ERROR_LOG("cannot receive cli = NULL.");
        return CRPC_OPERATE_NONE;
    }

    DEBUG_LOG("crpc operate: [%d]", ptr_crpc_msg->type);
    return ptr_crpc_msg->type;
}

/**
 * 说明：crpc方法调度函数
 * 返回：
 */
static int
crpc_operate_dispatch(crpc_cli_inst_t *cli, CrpcMessageHead *ptr_crpc_msg, CrpcMessageHead *ptr_crpc_msg_ack)
{
    int ret = ERROR;
    e_CrpcMsgType msg_type = CRPC_MSG_TYPE_NONE;
	CrpcMessageAck crpc_ack;
	
    CHECK_NULL_RETURN_ERROR(ptr_crpc_msg, "cannot accept crpc message is NULL.");

    msg_type = crpc_get_msg_type(ptr_crpc_msg);
    if (CRPC_MSG_TYPE_NONE == msg_type) {
        ERROR_LOG("crpc method code failed.");
        return OK;
    }

	crpc_message_ack__init(&crpc_ack);

    switch(msg_type) {
        case CRPC_MSG_TYPE_INSTALL:
            ret = crpc_operate_install(cli, ptr_crpc_msg);
            if (OK == ret) {
                DEBUG_LOG("crpc client [%s] install success!", cli->name);
            }
            else {
                ERROR_LOG("crpc client install failed!");
            }
			crpc_ack.result = ret;
			ptr_crpc_msg_ack->content.len = crpc_message_ack__get_packed_size(&crpc_ack);
			ptr_crpc_msg_ack->content.data = s_malloc_zero(ptr_crpc_msg_ack->content.len);
			crpc_message_ack__pack(&crpc_ack, crpc_msg_ack.content.data);
            break;
        case CRPC_MSG_TYPE_ACTIVE:
            ret = crpc_operate_activate(cli);
            if (OK == ret) {
                DEBUG_LOG("crpc client [%s] activate success!", cli->name);
            }
            else {
                ERROR_LOG("crpc client activate failed!");
            }
			crpc_ack.result = ret;
			ptr_crpc_msg_ack->content.len = crpc_message_ack__get_packed_size(&crpc_ack);
			ptr_crpc_msg_ack->content.data = s_malloc_zero(ptr_crpc_msg_ack->content.len);
			crpc_message_ack__pack(&crpc_ack, ptr_crpc_msg_ack->content.data);
            break;
        case CRPC_MSG_TYPE_CALLBACK:
			ret = crpc_operate_callback(cli, ptr_crpc_msg);
            if (OK == ret) {
                DEBUG_LOG("crpc client [%s] register success!", cli->name);
            }
            else {
                ERROR_LOG("crpc client register failed!");
            }
			crpc_ack.result = ret;
			crpc_msg_ack.content.len = crpc_message_ack__get_packed_size(&crpc_ack);
			crpc_msg_ack.content.data = s_malloc_zero(crpc_msg_ack.content.len);
			crpc_message_ack__pack(&crpc_ack, crpc_msg_ack.content.data);
			break;
		default:
			ERROR_LOG("crpc operate type is unknow. [%u]", msg_type);
    }

    return OK;
}
#endif
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
        recv_length = recv(inst->sk_fd, recv_buf, BUFFER_SIZE, 0);
        CHECK_ERROR_RETURN_ERROR(recv_length, "read() failed.");

        ret = buffer_append(inst->recv_buf, recv_buf, recv_length);
    } while (0 != recv_length);

    return OK;
}
/**
 * 说明：
 * 返回：
 * 备注：
 */

/**
 * 说明：发送crpc消息
 * 返回：
 * 备注：
 */
static int
crpc_srv_send_msg(crpc_cli_inst_t *cli)
{
    int ret = ERROR;
    buffer_t *send_buf;
    int32_t sent_len = ERROR;

    CHECK_NULL_RETURN_ERROR(cli, "cannot receive cli = NULL || buf = NULL.");

    send_buf = cli->send_buf;

    sent_len = send(cli->sk_fd, send_buf->data, send_buf->used, 0);
    CHECK_ERROR_RETURN_ERROR(sent_len, "write() to socket failed.");

    ret = buffer_flush(cli->send_buf);
    CHECK_ERROR_RETURN_ERROR(ret, "buffer_flush() failed.");

    DEBUG_LOG("crpc server send message to client: [%d] Byte.", sent_len);
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
    event_register.data.u32 = CRPC_EVENT_CLI & CRPC_EVENT_MASK;
    event_register.data.u32 |= cli->id & CRPC_INST_ID_MASK;
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
crpc_srv_recv_msg(crpc_cli_inst_t *cli)
{
    int ret = ERROR;
    uint8_t recv_buf[BUFFER_SIZE] = {0};
    int32_t recv_length = ERROR;

    CHECK_NULL_RETURN_ERROR(cli, "cannot receive cli = NULL || buf = NULL.");

    recv_length = read(cli->sk_fd, recv_buf, BUFFER_SIZE);
    CHECK_ZERO_RETURN_ERROR(recv_length, "read() failed.");

    ret = buffer_append(cli->recv_buf, recv_buf, recv_length);
    CHECK_OK_RETURN_RET(ret, "append receive buffer to client instance recv_buf failed.");

    DEBUG_LOG("reveive message from client: [%d] bytes.", recv_length);
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
    int ret = ERROR;
    list_node_t *iter = NULL;
    crpc_cli_inst_t *cli = NULL;
	CrpcMessageHead *ptr_crpc_msg_head;
	CrpcMessageHead crpc_msg_ack;

    for (iter = (srv->inst_list).head; NULL != iter; iter = iter->next) {
        cli = CONTAINER_OF(iter, crpc_cli_inst_t, list_node);
        if (cli_id == cli->id) {
            DEBUG_LOG("client awaken.");
            ret = crpc_srv_recv_msg(cli);
            CHECK_OK_RETURN_RET(ret, "receive message failed.");

			ptr_crpc_msg_head = crpc_message_head__unpack(NULL, cli->recv_buf->used, cli->recv_buf->data);
			CHECK_NULL_RETURN_ERROR(ptr_crpc_msg_head, "crpc msg unpack failed.");

			ret = buffer_flush(cli->recv_buf);
		    CHECK_ERROR_RETURN_ERROR(ret, "buffer_flush() failed.");
	
			if (CRPC_MAGIC != ptr_crpc_msg_head->magic) {
				ERROR_LOG("Message is not crpc type, magic = [%x]", ptr_crpc_msg_head->magic);
				return ERROR;
			}
			
            ret = crpc_operate_dispatch(cli, ptr_crpc_msg_head, &crpc_msg_ack);
            CHECK_OK_RETURN_RET(ret, "crpc operate dispatch failed.");

			crpc_msg_ack.magic = CRPC_MAGIC;
			crpc_msg_ack.type = CRPC_MSG_TYPE_CALLBACK;
			crpc_msg_ack.msg_id = CRPC_MSG_ID_GENERATE;
			crpc_msg_ack.name = strdup(cli->name);
			cli->send_buf->used = crpc_message_head__get_packed_size(&crpc_msg_ack);
			crpc_message_head__pack(&crpc_msg_ack, cli->send_buf->data);
			crpc_srv_send_msg(cli);

			crpc_message_head__free_unpacked(ptr_crpc_msg_head, NULL);
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
    strncpy(listen_addr.sun_path, UNIX_DOMAIN, sizeof(listen_addr.sun_path) - 1);
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
    event_register.data.u32 = CRPC_EVENT_SRV & CRPC_EVENT_MASK;
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
    uint32_t event_type = CRPC_EVENT_NONE;
    uint32_t cli_id = CRPC_INST_ID_NONE;
    struct epoll_event event_queue[20];

    for (;;) {
        event_num = epoll_wait(srv->ep_fd, event_queue, 20, 500);
        for (i = 0; i < event_num; i++) {
            event_type = event_queue[i].data.u32 & CRPC_EVENT_MASK;

            if (CRPC_EVENT_SRV == event_type) {
                crpc_srv_awaken(srv);
            } 
            else if (CRPC_EVENT_CLI == event_type) {
                cli_id = event_queue[i].data.u32 & CRPC_INST_ID_MASK;
                crpc_cli_awaken(srv, cli_id);
                cli_id = CRPC_INST_ID_NONE;
            } 
            else {
                /* 待定*/
                WARNING_LOG("unknow event type!");
            }
            event_type = CRPC_EVENT_NONE;
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
