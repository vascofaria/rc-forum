#ifndef __CLIENT_UDP_MANAGER_H__
#define __CLIENT_UDP_MANAGER_H__

#include "../user/user.h"

#define MAX_ARGS_N   3
#define MAX_ARGS_L   128
#define BUFFER_SIZE  1024

void
client_udp_manager(user_t *user, char *protocol, char args[MAX_ARGS_N][MAX_ARGS_L]);

#endif