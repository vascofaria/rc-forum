#ifndef __CLIENT_TCP_MANAGER_H__
#define __CLIENT_TCP_MANAGER_H__

#include "../entities/user.h"

#define MAX_ARGS_N   3
#define MAX_ARGS_L   128
#define BUFFER_SIZE  1024

void
client_tcp_manager(user_t *user, char* protocol, char args[MAX_ARGS_N][MAX_ARGS_L], int num_args);

#endif