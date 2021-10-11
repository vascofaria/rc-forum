#ifndef __CLIENT_MANAGER_H__ 
#define __CLIENT_MANAGER_H__

#include "../entities/user.h"

#define MAX_ARGS_N   3
#define MAX_ARGS_L   128

void 
client_manager(user_t *user, char* request);

#endif