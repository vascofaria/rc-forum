#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "user.h"

user_t* 
create_user () {
	user_t* user = (user_t*) malloc (sizeof(user_t));
	
	if (user != NULL) {
		user->user_id  = NULL;
		user->topic    = NULL;
		user->question = NULL;
		user->udp_addrinfo = NULL;
		user->tcp_addrinfo = NULL;
	}

	return user;
}

char*
get_user_id (user_t *user) {
	if (user) {
		return user->user_id;
	}

	return NULL;
}

char*
get_user_topic (user_t *user) {
	if (user) {
		return user->topic;	
	}

	return NULL;
}

char*
get_user_question (user_t *user) {
	if (user) {
		return user->question;
	}

	return NULL;
}

struct addrinfo* 
get_user_udp_addrinfo(user_t *user) {
	if (user) {
		return user->udp_addrinfo;
	}
	return NULL;
}

struct addrinfo* 
get_user_tcp_addrinfo(user_t *user) {
	if (user) {
		return user->tcp_addrinfo;
	}
	return NULL;
}

int 
get_user_server_sock_udp(user_t *user) {
	if (user) {
		return user->server_sock_udp;
	}
	return -1;
}

int 
get_user_server_sock_tcp(user_t *user) {
	if (user) {
		return user->server_sock_tcp;
	}
	return -1;
}

void 
set_user_id (user_t *user, char* id) {
	if (user) {
		if (user->user_id) {
			free(user->user_id);
			user->user_id = NULL;
		}
		user->user_id = strdup(id);
	}
}

void 
set_user_topic (user_t* user, char* topic) {
	if (user) {
		if (user->topic) {
			free(user->topic);
			user->topic = NULL;
		}
		user->topic = strdup(topic);
	}
}

void 
set_user_question (user_t* user, char* question) {
	if (user) {
		if (user->question) {
			free(user->question);
			user->question = NULL;
		}
		user->question = strdup(question);
	}
}

void 
set_user_udp_addrinfo(user_t *user, struct addrinfo *addrinfo) {
	if (user) {
		user->udp_addrinfo = addrinfo;
	}
}

void 
set_user_tcp_addrinfo(user_t *user, struct addrinfo *addrinfo) {
	if (user) {
		user->tcp_addrinfo = addrinfo;
	}
}

void
set_user_server_sock_udp(user_t *user, int sock_fd) {
	if (user) {
		user->server_sock_udp = sock_fd;
	}
}

void
set_user_server_sock_tcp(user_t *user, int sock_fd) {
	if (user) {
		user->server_sock_tcp = sock_fd;
	}
}

void 
clear_user_udp_addrinfo(user_t *user) {
	if (user) {
		user->udp_addrinfo = NULL;
	}
}

void 
clear_user_tcp_addrinfo(user_t *user) {
	if (user) {
		user->tcp_addrinfo = NULL;
	}
}

void
erase_user (user_t *user) {
	if (user) {
		if (user->user_id) {
			free(user->user_id);
			user->user_id = NULL;
		}

		if (user->topic) {
			free(user->topic);
			user->topic = NULL;
		}

		if (user->question) {	
			free(user->question);
			user->question = NULL;
		}

		free(user);
		user = NULL;
	}
}