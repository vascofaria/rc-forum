#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>

#include "../user/user.h"
#include "client-udp-manager.h"
#include "../error-messages/input-error-messages.h"

static char* make_new_REG_request(char *protocol, char *user_id) {
	char* request = (char *) malloc (sizeof(char) * (strlen(protocol) + strlen(user_id) + strlen(" ") + strlen("\n") + 1));
	if (request != NULL) {
		request[0] = '\0';
		strcpy(request, protocol);
		strcat(request, " ");
		strcat(request, user_id);
		strcat(request, "\n\0");
	}
	return request;
}

static char* make_new_LTP_request(char *protocol) {
	char* request = (char *) malloc (sizeof(char) * (strlen(protocol) + strlen(protocol) + strlen("\n") + 1));
	if (request != NULL) {
		request[0] = '\0';
		strcpy(request, protocol);
		strcat(request, "\n\0");
	}
	return request;	
}

static char* make_new_PTP_request(char *protocol, char *user_id, char *topic) {
	char* request = (char *) malloc (sizeof(char) * (strlen(protocol) + strlen(user_id) + strlen(topic) + (strlen(" ") * 2) + strlen("\n") + 1));
	if (request != NULL) {
		request[0] = '\0';
		strcpy(request, protocol);
		strcat(request, " ");
		strcat(request, user_id);
		strcat(request, " ");
		strcat(request, topic);
		strcat(request, "\n\0");
	}
	return request;
}

static char* make_new_LQU_request(char *protocol, char *topic) {
	char* request = (char *) malloc (sizeof(char) * (strlen(protocol) + strlen(topic) + strlen(" ") + strlen("\n") + 1));
	if (request != NULL) {
		request[0] = '\0';
		strcpy(request, protocol);
		strcat(request, " ");
		strcat(request, topic);
		strcat(request, "\n\0");
	}
	return request;
}

void
client_udp_manager(user_t *user, char* protocol, char args[MAX_ARGS_N][MAX_ARGS_L]) {
	char *request;
	if (!strcmp(protocol, "REG")) {
		request = make_new_REG_request(protocol, args[1]);
		if (request != NULL) {
			printf("%s %ld\n", request, strlen(request));
			sendto(get_user_server_sock_udp(user), request, strlen(request), 0, (struct sockaddr *) (get_user_udp_addrinfo(user)->ai_addr), (get_user_udp_addrinfo(user)->ai_addrlen));
			set_user_id(user, args[1]);
			free(request);
			request = NULL;
		}
	}

	else if (!strcmp(protocol, "LTP")) {
		request = make_new_LTP_request(protocol);	
		if (request != NULL) {
			printf("%s %ld\n", request, strlen(request));
			sendto(get_user_server_sock_udp(user), request, strlen(request), 0, (struct sockaddr *) (get_user_udp_addrinfo(user)->ai_addr), (get_user_udp_addrinfo(user)->ai_addrlen));
			free(request);
			request = NULL;
		}
	}

	else if (!strcmp(protocol, "PTP")) {
		if (get_user_id(user)) {
			request = make_new_PTP_request(protocol, get_user_id(user), args[1]);
			if (request != NULL) {
				printf("%s %ld\n", request, strlen(request));
				sendto(get_user_server_sock_udp(user), request, strlen(request), 0, (struct sockaddr *) (get_user_udp_addrinfo(user)->ai_addr), (get_user_udp_addrinfo(user)->ai_addrlen));
				free(request);
				request = NULL;			
			}
		}
		else {
			printf("%s\n", USER_IS_NOT_REGISTERED);
		}
	}

	else {
		if (get_user_topic(user)) {
			request = make_new_LQU_request(protocol, get_user_topic(user));
			if (request != NULL) {
				printf("%s %ld\n", request, strlen(request));
				sendto(get_user_server_sock_udp(user), request, strlen(request), 0, (struct sockaddr *) (get_user_udp_addrinfo(user)->ai_addr), (get_user_udp_addrinfo(user)->ai_addrlen));
				free(request);
				request = NULL;
			}
		}
		else {
			printf("%s\n", TOPIC_IS_NOT_SELECTED);
		}
	}
}