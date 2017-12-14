#include <sys/un.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>

#include "define.h"
#include "buffer.h"
#include "crpc_cli.h"

#define UNIX_DOMAIN ("/usr/local/etc/crpc/UNIX.domain")

int main(int argc, char **argv)
{
    int ret = ERROR;
    crpc_cli_t *crpc_cli = NULL;

    crpc_cli = crpc_cli_new();
    ret = crpc_cli_init(crpc_cli, "demo_cli1");

    crpc_cli_run(crpc_cli);

    crpc_cli_free(&crpc_cli);

    return 0;
}
