#ifndef _CRPC_CLI_H_

#include <stdbool.h>

#include "crpc_data.h"
#include "buffer.h"
#include "list.h"

#define UNIX_DOMAIN         ("/usr/local/etc/crpc/UNIX.domain")
#define BUFFER_SIZE         (1024)

extern crpc_cli_t *
crpc_cli_new();

extern int
crpc_cli_init(crpc_cli_t *cli, const char *name);

extern void
crpc_cli_free(crpc_cli_t **cli);

extern int
crpc_cli_init(crpc_cli_t *cli, const char *name);

extern void 
crpc_cli_run(crpc_cli_t *cli);


#endif //_CRPC_CLI_H
