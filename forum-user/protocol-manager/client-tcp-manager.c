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
	write_to_tcp_socket(sock_tcp, protocol, ' ');
	write_to_tcp_socket(sock_tcp, topic, ' ');
	write_to_tcp_socket(sock_tcp, question, '\n');
}

static void recv_GQU_request(user_t *user, char *question, int sock_tcp) {
	char path[MAX_PATH_SIZE];
	char protocol[PROTOCOL_SIZE];
	char q_user_id[USER_ID_SIZE], qsize[FILE_SIZE_DIGITS], qiext[EXTENSION_SIZE_DIGITS], qisize[FILE_SIZE_DIGITS], answers[MAX_ANSWERS_SIZE_DIGITS], qimg[2], c;
	char a_user_id[USER_ID_SIZE], asize[FILE_SIZE_DIGITS], aiext[EXTENSION_SIZE_DIGITS], aisize[FILE_SIZE_DIGITS], aimg[2];
	int  qsize_n, qisize_n, asize_n, aisize_n, answers_n, i, error_code, file_fd;

	/* Create Topic directory */
	/*error_code = mkdir(get_user_topic(user), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (error_code) {
		fprintf(stderr, "ERROR: Unable to create directory: %s: %s\n", get_user_topic(user), strerror(errno));
		exit(EXIT_FAILURE);
	}*/

	read_from_tcp_socket(sock_tcp, protocol, PROTOCOL_SIZE + 1, ' ');
	printf("%s\n", protocol);

	read_from_tcp_socket(sock_tcp, q_user_id, USER_ID_SIZE + 1, ' ');
	printf("%s\n", q_user_id);
	
	read_from_tcp_socket(sock_tcp, qsize, USER_ID_SIZE + 1, ' ');
	printf("%s\n", qsize);

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

static void send_QUS_request(int sock_tcp, char *protocol, char* user_id, char *topic, char *question, char *qsize, char *qpath, char *qimg, char *iext, char *isize, char *ipath) {
	write_to_tcp_socket(sock_tcp, protocol, ' ');
	write_to_tcp_socket(sock_tcp, user_id, ' ');
	write_to_tcp_socket(sock_tcp, topic, ' ');
	write_to_tcp_socket(sock_tcp, question, ' ');
	write_to_tcp_socket(sock_tcp, qsize, ' ');
	write_from_file_to_socket(sock_tcp, qpath, atoi(qsize));
	write_to_tcp_socket(sock_tcp, "\0", ' ');
	write_to_tcp_socket(sock_tcp, qimg, '\0');
	if (!strcmp(qimg, "1")) {
		write_to_tcp_socket(sock_tcp, "\0", ' ');
		write_to_tcp_socket(sock_tcp, iext, ' ');
		write_to_tcp_socket(sock_tcp, isize, ' ');
		write_from_file_to_socket(sock_tcp, ipath, atoi(isize));
	}
	write_to_tcp_socket(sock_tcp, "\0", '\n');
}

static void send_ANS_request(int sock_tcp, char *protocol, char* user_id, char *topic, char *question, char *asize, char *apath, char *aimg, char *iext, char *isize, char *ipath) {
	write_to_tcp_socket(sock_tcp, protocol, ' ');
	write_to_tcp_socket(sock_tcp, user_id, ' ');
	write_to_tcp_socket(sock_tcp, topic, ' ');
	write_to_tcp_socket(sock_tcp, question, ' ');
	write_to_tcp_socket(sock_tcp, asize, ' ');
	write_from_file_to_socket(sock_tcp, apath, atoi(asize));
	write_to_tcp_socket(sock_tcp, "\0", ' ');
	write_to_tcp_socket(sock_tcp, aimg, '\0');
	if (!strcmp(aimg, "1")) {
		write_to_tcp_socket(sock_tcp, "\0", ' ');
		write_to_tcp_socket(sock_tcp, iext, ' ');
		write_to_tcp_socket(sock_tcp, isize, ' ');
		write_from_file_to_socket(sock_tcp, ipath, atoi(isize));
	}
	write_to_tcp_socket(sock_tcp, "\0", '\n');
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
			set_user_question(user, args[1]);
			send_GQU_request(server_sock_tcp, protocol, get_user_topic(user), args[1]);
			//recv_GQU_request(user, args[1], server_sock_tcp);
		}
		else {
			printf("%s\n", TOPIC_IS_NOT_SELECTED);
		}
	}	
	
	else if (!strcmp(protocol, "QUS")) {
		if (get_user_id(user)) {
			if (get_user_topic(user)) {	
				size = get_file_size(args[2], "r");
				if (size) {
					if (num_args == 3) {
						send_QUS_request(server_sock_tcp, protocol, get_user_id(user), get_user_topic(user), args[1], size, args[2], "0", "", "", "");
					}
					else {
						isize = get_file_size(args[3], "r");
						iext  = get_file_ext(args[3]);
						if (isize && iext) {
							send_QUS_request(server_sock_tcp, protocol, get_user_id(user), get_user_topic(user), args[1], size, args[2], "1", iext, isize, args[3]);
						}
						if (iext) {
							free(iext);
							iext = NULL;
						}

						if (isize) {
							free(isize);
							isize = NULL;
						}
					}
					if (size) {
						free(size);
						size = NULL;
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
					size = get_file_size(args[1], "r");
					if (size) {
						if (num_args == 2) {
							send_ANS_request(server_sock_tcp, protocol, get_user_id(user), get_user_topic(user), get_user_question(user), size, args[1], "0", "", "", "");
						}
						else {
							isize = get_file_size(args[3], "r");
							iext  = get_file_ext(args[3]);
							if (isize && idata) {
								send_ANS_request(server_sock_tcp, protocol, get_user_id(user), get_user_topic(user), get_user_question(user), size, args[2], "1", iext, isize, idata);
							}
							if (iext) {
								free(iext);
								iext = NULL;
							}

							if (isize) {
								free(isize);
								isize = NULL;
							}
						}

						if (size) {
							free(size);
							size = NULL;
						}
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

	/* Close TCP socket */
	error_code = close(server_sock_tcp);
	
	if (error_code == -1) {
		fprintf(stderr, "close failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
}