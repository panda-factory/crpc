#ifndef _CRPC_SRV_H_

#include <stdbool.h>

#include "crpc_data.h"

#define UNIX_DOMAIN         ("/usr/local/etc/crpc/UNIX.domain")
#define BUFFER_SIZE         (1024)


crpc_srv_t *
crpc_srv_new();

void
crpc_srv_free(crpc_srv_t **srv);

extern int
crpc_srv_init(crpc_srv_t *crpc_srv, const char *name)__attribute__ ((warn_unused_result));

extern void
crpc_srv_run(crpc_srv_t *crpc_srv);

#endif //_CRPC_SRV_H
