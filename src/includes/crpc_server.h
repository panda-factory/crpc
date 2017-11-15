#ifndef _CRPC_H_

#include "buffer.h"
#include "list.h"

#define UNIX_DOMAIN         ("/usr/local/etc/crpc/UNIX.domain")
#define BUFFER_SIZE         (1024)

typedef struct _crpc_cli_info_t
{
    list_node_t list_node;

    int sk_fd;

    buffer_t *send_buf;
    buffer_t *recv_buf;
} crpc_cli_info_t;

extern void
crpc_init();

extern void
crpc_run();
#endif //_CRPC_H
