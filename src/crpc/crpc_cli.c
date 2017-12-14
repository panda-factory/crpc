#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/un.h>

#include "tlv.h"
#include "crpc_cli.h"
#include "define.h"
#include "security.h"
#include "buffer.h"


#if DESC("内部函数")
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
crpc_cli_send_msg(crpc_cli_t *cli)
{
    int ret = ERROR;
    void *send_buf = NULL;
    uint32_t send_length = 0;
    int64_t sent_length = ERROR;

    CHECK_NULL_RETURN_ERROR(cli, "cannot receive cli = NULL || buf = NULL.");

    send_buf = cli->send_buf;
    send_length = buffer_used((buffer_t *)send_buf);

    sent_length = write(cli->sk_fd, send_buf, send_length);
    CHECK_ERROR_RETURN_ERROR(sent_length, "write() to socket failed.");

    ret = buffer_flush(cli->send_buf);
    CHECK_ERROR_RETURN_ERROR(ret, "buffer_flush() failed.");

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
 * 说明：填充crpc注册报文消息体
 * 返回：
 * 备注：
 */
static int
crpc_fill_register_msg(crpc_cli_t *cli)
{
    int ret = ERROR;
    tlv_t *tlv = NULL;

    CHECK_NULL_RETURN_ERROR(cli, "cannot receive cli = NULL.");

    tlv = tlv_new(CLIENT_NAME, strlen(cli->name) + 1, cli->name);
    TLV_CHECK_NULL_RETURN_ERROR(tlv, "tlv_new() failed.");
    ret = buffer_append(&cli->send_buf, tlv, sizeof(tlv_t) + tlv_length(tlv));
    TLV_CHECK_ERROR_RETURN_ERROR(ret, "buffer_apppend() failed.");

    ret = tlv_renew(&tlv, TERMINATOR, 0, NULL);
    TLV_CHECK_ERROR_RETURN_ERROR(ret, "tlv_new() failed.");
    ret = buffer_append(&cli->send_buf, tlv, sizeof(tlv_t) + tlv_length(tlv));
    TLV_CHECK_ERROR_RETURN_ERROR(ret, "buffer_apppend() failed.");

    return OK;
}

/**
 * 说明：填充crpc激活报文消息体
 * 返回：
 * 备注：
 */
static int
crpc_fill_activate_msg(crpc_cli_t *cli)
{
    int ret = ERROR;

    CHECK_NULL_RETURN_ERROR(cli, "cannot receive cli = NULL.");

    return OK;
}

/**
 * 说明：填充crpc报文头
 * 返回：
 * 备注：
 */
static int
crpc_fill_msg_head(buffer_t **buf, e_crpc_method method)
{
    int ret = ERROR;
    crpc_msg_head_t msg_head;

    CHECK_2_NULL_RETURN_ERROR(buf, *buf, "cannot receive buf = NULL.");

    msg_head.magic = CRPC_MAGIC;
    msg_head.method = method;
    msg_head.length = 0;

    ret = buffer_append(buf, &msg_head, sizeof(crpc_msg_head_t));
    if (ERROR == ret) {
        ERROR_LOG("buffer_append() failed.");
        return ERROR;
    }

    return OK;
}

/**
 * 说明：构造crpc注册报文
 * 返回：
 * 备注：
 */
static int
crpc_build_register_msg(crpc_cli_t *cli)
{
    int ret = ERROR;
    crpc_msg_head_t *msg_head = NULL;

    CHECK_NULL_RETURN_ERROR(cli, "cannot receive cli = NULL");

    ret = crpc_fill_msg_head(&cli->send_buf, METHOD_REGISTER);
    CHECK_ERROR_RETURN_ERROR(ret, "crpc_fill_msg_head() failed.");

    ret = crpc_fill_register_msg(cli);
    CHECK_ERROR_RETURN_ERROR(ret, "crpc_fill_register_msg() failed.");

    msg_head = (crpc_msg_head_t *)buffer_data(cli->send_buf);
    msg_head->length = buffer_used(cli->send_buf) - sizeof(crpc_msg_head_t);

    return OK;
}

/**
 * 说明：构造crpc激活报文
 * 返回：
 * 备注：
 */
static int
crpc_build_activate_msg(crpc_cli_t *cli)
{
    int ret = ERROR;
    crpc_msg_head_t *msg_head = NULL;

    CHECK_NULL_RETURN_ERROR(cli, "cannot receive cli = NULL");

    ret = crpc_fill_msg_head(&cli->send_buf, METHOD_ACTIVATE);
    CHECK_ERROR_RETURN_ERROR(ret, "crpc_fill_msg_head() failed.");

    ret = crpc_fill_register_msg(cli);
    CHECK_ERROR_RETURN_ERROR(ret, "crpc_fill_register_msg() failed.");

    msg_head = (crpc_msg_head_t *)buffer_data(cli->send_buf);
    msg_head->length = buffer_used(cli->send_buf) - sizeof(crpc_msg_head_t);

    return OK;
}

/**
 * 说明：crpc注册
 * 返回：
 * 备注：
 */
static int
crpc_method_register(crpc_cli_t *cli)
{
    int ret = ERROR;
    const tlv_t *iter = NULL;

    CHECK_NULL_RETURN_ERROR(cli, "input param crpc_cli = NULL,");

    ret = crpc_build_register_msg(cli);
    CHECK_ERROR_RETURN_ERROR(ret, "crpc_build_msg() failed.");

    ret = crpc_cli_send_msg(cli);
    CHECK_ERROR_RETURN_ERROR(ret, "crpc_cli_send_msg() failed.");

    ret = crpc_cli_recv_msg(cli);
    CHECK_ERROR_RETURN_ERROR(ret, "crpc_cli_recv_msg() failed.");

    DEBUG_LOG("crpc client [%s] register success.", cli->name);

    return OK;
}

/**
 * 说明：crpc激活
 * 返回：
 * 备注：
 */
static int
crpc_method_activate(crpc_cli_t *cli)
{
    int ret = ERROR;
    const tlv_t *iter = NULL;

    CHECK_NULL_RETURN_ERROR(cli, "input param crpc_cli = NULL,");

    ret = crpc_build_activate_msg(cli);
    CHECK_ERROR_RETURN_ERROR(ret, "crpc_build_msg() failed.");

    ret = crpc_cli_send_msg(cli);
    CHECK_ERROR_RETURN_ERROR(ret, "crpc_cli_send_msg() failed.");

    ret = crpc_cli_recv_msg(cli);
    CHECK_ERROR_RETURN_ERROR(ret, "crpc_cli_recv_msg() failed.");

    DEBUG_LOG("crpc client [%s] register success.", cli->name);

    return OK;
}

/**
 * 说明：crpc方法调度函数
 * 返回：
 */
static int
crpc_method_dispatch(crpc_cli_t *crpc_cli)
{
    return OK;
}

/**
 * 说明：crpc客户端sk_fd初始化
 * 返回：
 * 备注：
 */
static int
crpc_cli_fd_init(crpc_cli_t *cli)
{
    int ret = ERROR;
    struct sockaddr_un cli_addr;

    cli->sk_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (cli->sk_fd < 0) {
        perror("cannot create client socket.");
        return ERROR;
    }

    cli_addr.sun_family = AF_UNIX;
    strcpy(cli_addr.sun_path, UNIX_DOMAIN);

    ret = connect(cli->sk_fd, (struct sockaddr *)&cli_addr, sizeof(struct sockaddr_un));
    if (-1 == ret) {
         perror("cannot connect to the server");
         return ERROR;
     }

    return OK;
}

#endif //内部函数

#if DESC("外部函数")

/**
 * 说明：申请crpc客户端内存，并初始化
 * 返回：crpc_cli_t *
 * 备注：返回值外部释放
 */
crpc_cli_t *
crpc_cli_new()
{
    int ret = ERROR;
    crpc_cli_t *cli = NULL;

    cli = (crpc_cli_t *)malloc(sizeof(crpc_cli_t));
    CHECK_NULL_RETURN_NULL(cli, "malloc failed.");
    ret = s_memset(cli, sizeof(crpc_cli_t), 0, sizeof(crpc_cli_t));
    if (ERROR == ret) {
        ERROR_LOG("s_memset() failed.");
        s_free(cli);
        return NULL;
    }

    return cli;
}

/**
 * 说明：初始化crpc客户端实例
 * 返回：
 * 备注：
 */
int
crpc_cli_init(crpc_cli_t *cli, const char *name)
{
    int ret = ERROR;

    CHECK_2_NULL_RETURN_ERROR(cli, name, "input params cannot be NULL.");

    cli->name = strdup("name");
    CHECK_NULL_RETURN_ERROR(cli->name, "strdup() failed.");

    ret = crpc_cli_fd_init(cli);
    CHECK_ERROR_RETURN_ERROR(ret, "crpc_cli_fd_init() failed.");

    cli->send_buf = buffer_new(BUFFER_SIZE);
    cli->recv_buf = buffer_new(BUFFER_SIZE);

    DEBUG_LOG("crpc client initiate success!.");
    return OK;
}

/**
 * 说明：释放crpc客户端实例内存，并将指针赋值NULL
 * 返回：
 * 备注：
 */
void
crpc_cli_free(crpc_cli_t **cli)
{
    if (NULL == cli || NULL == *cli) {
        return;
    }

    s_free((*cli)->name);
    s_free(*cli);

    return;
}

/**
 * 说明：运行crpc客户端
 * 返回：
 * 备注：
 */
void 
crpc_cli_run(crpc_cli_t *cli)
{
    int ret = ERROR;
    
    ret = crpc_method_register(cli);
    ret = crpc_method_activate(cli);
    return;
}


#endif //外部函数