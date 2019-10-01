#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "client-tcp-manager.h"
#include "../error-messages/input-error-messages.h"
#include "../file-manager/file-manager.h"

static char* make_new_GQU_request(char *protocol, char *topic, char *question) {
	char *request = (char *) malloc (sizeof(char) * (strlen(protocol) + strlen(topic) + strlen(question) + (strlen(" ") * 2) + strlen("\n") + 1));
	if (request != NULL) {
		request[0] = '\0';
		strcpy(request, protocol);
		strcat(request, " ");
		strcat(request, topic);
		strcat(request, " ");
		strcat(request, question);
		strcat(request, "\n\0");
	}
	return request;
}

static char* make_new_QUS_request(char *protocol, char* user_id, char *topic, char *question, char *qsize, char *qdata, char *qimg, char *iext, char *isize, char *idata) {
	int   img_size, i = 0, j;
	char *request;

	request = (char *) malloc (sizeof(char) * (
			strlen(protocol) + strlen(user_id) + strlen(topic) + strlen(question) + strlen(qsize) +
			strlen(qdata)    + strlen(qimg)    + strlen(iext)  + strlen(isize)    + atoi(isize)   +
			+ (9 * strlen(" ")) + strlen("\n") + strlen("\0")));
	if (request != NULL) {
		request[0] = '\0';
		strcpy(request, protocol);
		strcat(request, " ");
		strcat(request, user_id);
		strcat(request, " ");
		strcat(request, question);
		strcat(request, " ");
		strcat(request, qsize);
		strcat(request, " ");
		strcat(request, qdata);
		strcat(request, " ");
		strcat(request, qimg);
		if (!strcmp(qimg, "1")) {
			img_size = atoi(isize);
			strcat(request, " ");
			strcat(request, iext);
			strcat(request, " ");
			strcat(request, isize);
			strcat(request, " ");
			for (i = strlen(request), j = 0; j < img_size; i++, j++) {
				request[i] = idata[j];
			}
		}
		strcat(request, "\n\0");
	}
	return request;
}

static char* make_new_ANS_request(char *protocol, char* user_id, char *topic, char *question, char *asize, char *adata, char *aimg, char *iext, char *isize, char *idata) {
	int   img_size, i = 0, j;
	char *request;
	
	request = (char *) malloc (sizeof(char) * (
			strlen(protocol) + strlen(user_id) + strlen(topic) + strlen(question) + strlen(asize) +
			strlen(adata)    + strlen(aimg)    + strlen(iext)  + strlen(isize)    + atoi(isize)   +
			+ (9 * strlen(" ")) + strlen("\n") + strlen("\0")));
	if (request != NULL) {
		request[0] = '\0';
		strcpy(request, protocol);
		strcat(request, " ");
		strcat(request, user_id);
		strcat(request, " ");
		strcat(request, question);
		strcat(request, " ");
		strcat(request, asize);
		strcat(request, " ");
		strcat(request, adata);
		strcat(request, " ");
		strcat(request, aimg);
		if (!strcmp(aimg, "1")) {
			img_size = atoi(isize);
			strcat(request, " ");
			strcat(request, iext);
			strcat(request, " ");
			strcat(request, isize);
			strcat(request, " ");
			for (i = strlen(request), j = 0; j < img_size; i++, j++) {
				request[i] = idata[j];
			}
		}
		strcat(request, "\n\0");
	}
	return request;
}

static void executes_tcp_command(char *request, struct addrinfo *tcp_addrinfo) {
	int server_sock_tcp, error_code;
	char *buffer = (char *) malloc (sizeof(char) * (BUFFER_SIZE + 1));

	if (buffer == NULL) {
		return;
	}

	/* Socket TCP creation */
	server_sock_tcp = socket(tcp_addrinfo->ai_family, tcp_addrinfo->ai_socktype, tcp_addrinfo->ai_protocol);

	if (server_sock_tcp == -1) {
		fprintf(stderr, "socket creation failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	/*			           */

	/* Connect to TCP socket */
	error_code = connect(server_sock_tcp, tcp_addrinfo->ai_addr, tcp_addrinfo->ai_addrlen);

	if (error_code == -1) {
		fprintf(stderr, "connect failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* Write on server socket */
	write(server_sock_tcp, request, strlen(request) + 1);

	/* Read from server socket */
	read(server_sock_tcp, buffer, BUFFER_SIZE);

	error_code = close(server_sock_tcp);
	if (error_code == -1) {
		fprintf(stderr, "close failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
}

void
client_tcp_manager(user_t *user, char* protocol, char args[MAX_ARGS_N][MAX_ARGS_L], int num_args) {
	char *request, *size, *data, *img, *iext, *isize, *idata;
	if (!strcmp(protocol, "GQU")) {
		if (get_user_topic(user)) {
			request = make_new_GQU_request(protocol, get_user_topic(user), args[1]);
			
			if (request) {
				executes_tcp_command(request, get_user_tcp_addrinfo(user));
				free(request);
				request = NULL;
			}
		}
		else {
			printf("%s\n", TOPIC_IS_NOT_SELECTED);
		}
	}	

	else if (!strcmp(protocol, "QUS")) {
		if (get_user_id(user)) {
			if (get_user_topic(user)) {	
				size = get_file_size(args[2], "r");
				data = get_file_data(args[2], "r");
				if (num_args == 3) {
					request = make_new_QUS_request(protocol, get_user_id(user), get_user_topic(user), args[1], size, data, "0", "", "", "");
				}
				else {
					isize = get_file_size(args[3], "rb");
					idata = get_file_data(args[3], "rb");
					iext  = get_file_ext(args[3]);
					if (isize && idata && iext) {
						request = make_new_QUS_request(protocol, get_user_id(user), get_user_topic(user), args[1], size, data, "1", iext, isize, idata);
					}
					if (iext) {
						free(iext);
						iext = NULL;
					}

					if (isize) {
						free(isize);
						isize = NULL;
					}
					if (idata) {
						free(idata);
						idata = NULL;
					}
				}
				if (size) {
					free(size);
					size = NULL;
				}
				if (data) {
					free(data);
					data = NULL;
				}
				if (request) {
					executes_tcp_command(request, get_user_tcp_addrinfo(user));
					free(request);
					request = NULL;
				}
			}
			else {
				printf("%s\n", TOPIC_IS_NOT_SELECTED);
			}
		}
		else {
			printf("%s\n", USER_IS_NOT_REGISTERED);
		}
	}

	else {
		if (get_user_id(user)) {
			if (get_user_topic(user)) {
				if (get_user_question(user)) {	
					size = get_file_size(args[2], "r");
					data = get_file_size(args[2], "r");
					if (num_args == 2) {
						request = make_new_ANS_request(protocol, get_user_id(user), get_user_topic(user), get_user_question(user), size, data, "0", "", "", "");
					}
					else {
						isize = get_file_size(args[2], "rb");
						idata = get_file_data(args[2], "rb");
						iext  = get_file_ext(args[2]);
						if (isize && idata) {
							request = make_new_ANS_request(protocol, get_user_id(user), get_user_topic(user), get_user_question(user), size, data, "1", iext, isize, idata);
						}
						if (iext) {
							free(iext);
							iext = NULL;
						}

						if (isize) {
							free(isize);
							isize = NULL;
						}
						if (idata) {
							free(idata);
							idata = NULL;
						}
					}

					if (size) {
						free(size);
						size = NULL;
					}
					if (data) {
						free(data);
						data = NULL;
					}
					if (request) {
						executes_tcp_command(request, get_user_tcp_addrinfo(user));
						free(request);
						request = NULL;
					}
				}
				else {
					printf("%s\n", QUESTION_IS_NOT_SELECTED);
				}
			}
			else {
				printf("%s\n", TOPIC_IS_NOT_SELECTED);
			}
		}
		else {
			printf("%s\n", USER_IS_NOT_REGISTERED);
		}
	}
}