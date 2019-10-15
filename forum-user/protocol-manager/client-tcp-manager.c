#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../constants.h"
#include "client-tcp-manager.h"
#include "../file-manager/file-manager.h"
#include "../error-messages/input-error-messages.h"

static void send_GQU_request(int sock_tcp, char *protocol, char *topic, char *question) {
	write_to_tcp_socket(sock_tcp, protocol);
	write_to_tcp_socket(sock_tcp, " ");
	write_to_tcp_socket(sock_tcp, topic);
	write_to_tcp_socket(sock_tcp, " ");
	write_to_tcp_socket(sock_tcp, question);
	write_to_tcp_socket(sock_tcp, "\n\0");
}

static void recv_GQU_request(user_t *user, char *question, int sock_tcp) {
	char path[MAX_PATH_SIZE];
	char protocol[PROTOCOL_SIZE];
	char q_user_id[USER_ID_SIZE], qsize[FILE_SIZE_DIGITS], qiext[EXTENSION_SIZE_DIGITS], qisize[FILE_SIZE_DIGITS], answers[MAX_ANSWERS_SIZE_DIGITS], qimg[2], c;
	char a_user_id[USER_ID_SIZE], asize[FILE_SIZE_DIGITS], aiext[EXTENSION_SIZE_DIGITS], aisize[FILE_SIZE_DIGITS], aimg[2];
	int  qsize_n, qisize_n, asize_n, aisize_n, answers_n, i, error_code, file_fd;

	/* Create Topic directory */
	error_code = mkdir(get_user_topic(user), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (error_code) {
		fprintf(stderr, "ERROR: Unable to create directory: %s: %s\n", get_user_topic(user), strerror(errno));
		exit(EXIT_FAILURE);
	}

	read_from_tcp_socket(sock_tcp, protocol, PROTOCOL_SIZE + 1, ' ');

	read_from_tcp_socket(sock_tcp, q_user_id, USER_ID_SIZE + 1, ' ');
	
	read_from_tcp_socket(sock_tcp, qsize, USER_ID_SIZE + 1, ' ');
		
	qsize_n = atoi(qsize);

	strcpy(path, question);
	strcat(path, ".txt\0");

	write_from_socket_to_file(sock_tcp, path, qsize_n);
	
	read_from_tcp_socket(sock_tcp, qimg, 1 + 1, ' ');
	
	read_from_tcp_socket(sock_tcp, &c, 1, ' ');

	if (!strcmp(qimg, "1")) {
		read_from_tcp_socket(sock_tcp, qiext, EXTENSION_SIZE_DIGITS + 1, ' ');

		read_from_tcp_socket(sock_tcp, qisize, FILE_SIZE_DIGITS + 1, ' ');

		qisize_n = atoi(qsize);

		write_from_socket_to_file(sock_tcp, "ola", qisize_n);
	}

	read_from_tcp_socket(sock_tcp, answers, MAX_ANSWERS_SIZE_DIGITS + 1, ' ');

	answers_n = atoi(answers);

	for (i = 0; i < answers_n; i++) {
		read_from_tcp_socket(sock_tcp, answers, MAX_ANSWERS_SIZE_DIGITS, ' ');
		
		read_from_tcp_socket(sock_tcp, a_user_id, USER_ID_SIZE + 1, ' ');
	
		read_from_tcp_socket(sock_tcp, asize, USER_ID_SIZE + 1, ' ');
		
		asize_n = atoi(asize);

		write_from_socket_to_file(sock_tcp, "ola", asize_n);
	
		read_from_tcp_socket(sock_tcp, aimg, 1 + 1, ' ');
	
		read_from_tcp_socket(sock_tcp, &c, 1, ' ');

		if (!strcmp(aimg, "1")) {
			read_from_tcp_socket(sock_tcp, aiext, EXTENSION_SIZE_DIGITS + 1, ' ');

			read_from_tcp_socket(sock_tcp, aisize, FILE_SIZE_DIGITS + 1, ' ');

			aisize_n = atoi(asize);

			write_from_socket_to_file(sock_tcp, "ola", aisize_n);
		}	
	}
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

void client_tcp_manager(user_t *user, char* protocol, char args[MAX_ARGS_N][MAX_ARGS_L], int num_args) {
	int   error_code, server_sock_tcp;
	char *request, *size, *data, *img, *iext, *isize, *idata;
	
	/* Socket TCP creation */
	server_sock_tcp = socket(get_user_tcp_addrinfo(user)->ai_family, get_user_tcp_addrinfo(user)->ai_socktype, get_user_tcp_addrinfo(user)->ai_protocol);

	if (server_sock_tcp == -1) {
		fprintf(stderr, "socket creation failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* Connect to TCP socket */
	error_code = connect(server_sock_tcp, get_user_tcp_addrinfo(user)->ai_addr, get_user_tcp_addrinfo(user)->ai_addrlen);

	if (error_code == -1) {
		fprintf(stderr, "connect failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	if (!strcmp(protocol, "GQU")) {
		if (get_user_topic(user)) {
			send_GQU_request(server_sock_tcp, protocol, get_user_topic(user), args[1]);
			recv_GQU_request(user, args[1], server_sock_tcp);
		}
		else {
			printf("%s\n", TOPIC_IS_NOT_SELECTED);
		}
	}	
	/*
	else if (!strcmp(protocol, "QUS")) {
		if (get_user_id(user)) {
			if (get_user_topic(user)) {	
				size = get_file_size(args[2], "r");
				data = get_file_data(args[2], "r");
				if (size && data) {
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
	}*/

	/* Close TCP socket */
	error_code = close(server_sock_tcp);
	
	if (error_code == -1) {
		fprintf(stderr, "close failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
}