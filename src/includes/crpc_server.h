#ifndef _CRPC_H_

#include <stdbool.h>

#include "buffer.h"
#include "list.h"

#define UNIX_DOMAIN         ("/usr/local/etc/crpc/UNIX.domain")
#define BUFFER_SIZE         (1024)

typedef struct _crpc_msg_head_t
{
    uint32_t magic;

} crpc_msg_head_t;

typedef struct _crpc_plugin_inst_t
{
    list_node_t list_node;

    char *name;
    int sk_fd;

    bool flag_register;
    bool flag_activate;

    int (*method_register)(struct _crpc_plugin_inst_t *plugin_inst);
    buffer_t *send_buf;
    buffer_t *recv_buf;
} crpc_plugin_inst_t;

extern void
crpc_init();

extern void
crpc_run();
#endif //_CRPC_H
