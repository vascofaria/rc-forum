#ifndef __USER_H__
#define __USER_H__

typedef struct user {
	char*  user_id;
	char*  topic;
	char*  question;
	struct addrinfo *udp_addrinfo, *tcp_addrinfo;
	int    server_sock_udp, server_sock_tcp;
} user_t;

user_t* 
create_user ();

char*
get_user_id (user_t *user);

char*
get_user_topic (user_t *user);

char*
get_user_question (user_t *user);

struct addrinfo* 
get_user_udp_addrinfo(user_t *user);

struct addrinfo* 
get_user_tcp_addrinfo(user_t *user);

int 
get_user_server_sock_udp(user_t *user);

int 
get_user_server_sock_tcp(user_t *user);

void 
set_user_id (user_t *user, char* id);

void 
set_user_topic (user_t* user, char* topic);

void 
set_user_question (user_t* user, char* question);

void 
set_user_udp_addrinfo(user_t *user, struct addrinfo *addrinfo);

void 
set_user_tcp_addrinfo(user_t *user, struct addrinfo *addrinfo);

void
set_user_server_sock_udp(user_t *user, int sock_fd);

void
set_user_server_sock_tcp(user_t *user, int sock_fd);

void 
clear_user_udp_addrinfo(user_t *user);

void 
clear_user_tcp_addrinfo(user_t *user);

void
erase_user (user_t *user);

#endif