#ifndef _CRPC_DATA_H_
#define _CRPC_DATA_H_

#include "buffer.h"
#include "list.h"

#define CRPC_MAGIC          (0XFEEDBEEF)
#define CRPC_EVENT_MASK     (0XFFFF0000)
#define CRPC_EVENT_NONE     (0X00000000)
#define CRPC_EVENT_SRV      (0X00010000)
#define CRPC_EVENT_CLI      (0X00020000)
#define CRPC_INST_ID_MASK   (0X0000FFFF)
#define CRPC_INST_ID_NONE   (0X00000000)

#define CRPC_METHOD_HELLOWORLD CRPC_METHOD_NONE

typedef enum _e_CrpcMsgType
{
	CRPC_MSG_TYPE_NONE = 0,
	CRPC_MSG_TYPE_INSTALL,
	CRPC_MSG_TYPE_ACTIVE,
	CRPC_MSG_TYPE_REGISTER,
	CRPC_MSG_TYPE_BUTT
} e_CrpcMsgType;

typedef enum _e_crpc_operate
{
    CRPC_OPERATE_NONE = 0,
    CRPC_OPERATE_INSTALL,
    CRPC_OPERATE_ACTIVATE,
    CRPC_OPERATE_REGISTE,
    CRPC_OPERATE_CALLBACK,
    CRPC_OPERATE_KICKOFF,
    CRPC_OPERATE_BUTT,
} e_crpc_operate;

typedef enum _e_CrpcCallbackID
{
    CRPC_CALLBACK_NONE = 0,
    CRPC_CALLBACK_REGISTER,
    CRPC_CALLBACK_BUTT,
} e_CrpcCallbackID;


typedef enum _e_crpc_tlv
{
    NONE = 0,
    CLIENT_NAME,
    TERMINATOR,
} e_crpc_tlv;

typedef int (*CrpcMethod)(void *, ...);

typedef struct _crpc_msg_head_t
{
    uint32_t        magic;
    uint32_t        length;
    e_crpc_operate   operate;
} crpc_msg_head_t;

typedef struct _crpc_cli_t
{
    list_node_t list_node;

    char *name;
    int sk_fd;

    bool flag_register;
    bool flag_activate;

    int (*method_register)(struct _crpc_cli_t *crpc_cli);
    buffer_t *send_buf;
    buffer_t *recv_buf;

    CrpcMethod *method;
} crpc_cli_t;

typedef struct _crpc_cli_inst_t
{
    list_node_t list_node;

    char        *name;
    uint32_t    id;
    int         sk_fd;

    bool        flag_install;
    bool        flag_activate;

    int (*method_register)(struct _crpc_cli_t *crpc_cli);
    buffer_t    *send_buf;
    buffer_t    *recv_buf;

    CrpcMethod *callback;
} crpc_cli_inst_t;

typedef struct _crpc_srv_t
{
    char *name;
    int sk_fd;
    int ep_fd;

    uint32_t    count_inst;

    list_table_t    inst_list;
} crpc_srv_t;

#define CRPC_MSG_ID_GENERATE	0

#endif //_CRPC_DATA_H_
