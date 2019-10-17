#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../constants.h"
#include "../exceptions.h"
#include "client-tcp-manager.h"
#include "../file-manager/file-manager.h"
#include "../error-messages/input-error-messages.h"
#include "../error-messages/server-error-messages.h"

static void send_GQU_request(int sock_tcp, char *protocol, char *topic, char *question) {
	write_to_tcp_socket(sock_tcp, protocol, ' ');
	write_to_tcp_socket(sock_tcp, topic, ' ');
	write_to_tcp_socket(sock_tcp, question, '\n');
}

static void recv_GQU_request(user_t *user, char *question, int sock_tcp) {
	char path[MAX_PATH_SIZE];
	char protocol[PROTOCOL_SIZE + 1];
	char q_user_id[USER_ID_SIZE + 1], qsize[FILE_SIZE_DIGITS + 1], qiext[EXTENSION_SIZE_DIGITS + 1], qisize[FILE_SIZE_DIGITS + 1], answers[MAX_ANSWERS_SIZE_DIGITS + 1], qimg, c;
	char a_user_id[USER_ID_SIZE + 1], asize[FILE_SIZE_DIGITS + 1], aiext[EXTENSION_SIZE_DIGITS + 1], aisize[FILE_SIZE_DIGITS + 1], aimg;
	int  qsize_n, qisize_n, asize_n, aisize_n, answers_n, i, error_code, file_fd;

	if (access(get_user_topic(user), F_OK) != 0) {
		/* Create Topic directory */
		error_code = mkdir(get_user_topic(user), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		if (error_code) {
			fprintf(stderr, "ERROR: Unable to create directory: %s: %s\n", get_user_topic(user), strerror(errno));
			exit(EXIT_FAILURE);
		}	
	}

	error_code = read_from_tcp_socket(sock_tcp, protocol, PROTOCOL_SIZE + 1, ' ');
	printf("%s\n", protocol);
	if (error_code) {
		printf("Errro\n");
	}


	error_code = read_from_tcp_socket(sock_tcp, q_user_id, USER_ID_SIZE + 1, ' ');
	printf("%s\n", q_user_id);
	if (error_code) {
		printf("Errro\n");
	}


	error_code =  read_from_tcp_socket(sock_tcp, qsize, MAX_QUESTIONS_SIZE_DIGITS + 1, ' ');
	printf("%s\n", qsize);
	if (error_code) {
		printf("Errro\n");
	}


	qsize_n = atoi(qsize);

	strcpy(path, "./");
	strcat(path, get_user_topic(user));
	strcat(path, "/");
	strcat(path, question);
	strcat(path, ".txt\0");
	printf("%s\n", path);
	write_from_socket_to_file(sock_tcp, path, qsize_n);
	
	error_code =  read_from_tcp_socket(sock_tcp, NULL, 0, ' ');
	if (error_code) {
		printf("Errro1\n");
	}

	error_code = read_from_tcp_socket(sock_tcp, &qimg, 1, '\0');
	printf("%c\n", qimg);
	if (error_code) {
		printf("Errro2\n");
	}


	error_code = read_from_tcp_socket(sock_tcp, NULL, 0, ' ');
	if (error_code) {
		printf("Errro3\n");
	}


	if (qimg == '1') {
		error_code = read_from_tcp_socket(sock_tcp, qiext, EXTENSION_SIZE_DIGITS + 1, ' ');
		if (error_code) {
			printf("Errro4\n");
		}
		printf("%s\n", qiext);
		error_code = read_from_tcp_socket(sock_tcp, qisize, FILE_SIZE_DIGITS + 1, ' ');
		if (error_code) {
			printf("Errro5\n");
		}
		printf("%s\n", qisize);
		qisize_n = atoi(qisize);
		
		strcpy(path, "./");
		strcat(path, get_user_topic(user));
		strcat(path, "/");
		strcat(path, question);
		strcat(path, ".");
		strcat(path, qiext);
		printf("%s\n", path);
		
		write_from_socket_to_file(sock_tcp, path, qisize_n);
		error_code = read_from_tcp_socket(sock_tcp, NULL, 0, ' ');
		if (error_code) {
			printf("Errro6\n");
		}
	}

	error_code = read_from_tcp_socket(sock_tcp, answers, MAX_ANSWERS_SIZE_DIGITS, '\0');
	if (error_code) {
		printf("Errro7\n");
	}
	printf("%s\n", answers);
	answers_n = atoi(answers);

	for (i = 0; i < answers_n; i++) {
		printf("ss\n");
		error_code = read_from_tcp_socket(sock_tcp, NULL, 0, ' ');
		if (error_code) {
			printf("Errro8\n");
		}

		error_code = read_from_tcp_socket(sock_tcp, answers, MAX_ANSWERS_SIZE_DIGITS + 1, ' ');
		if (error_code) {
			printf("Errro9\n");
		}
		printf("%s\n", answers);
		error_code =  read_from_tcp_socket(sock_tcp, a_user_id, USER_ID_SIZE + 1, ' ');
		if (error_code) {
			printf("Errro10\n");
		}
		printf("%s\n", a_user_id);
		error_code = read_from_tcp_socket(sock_tcp, asize, FILE_SIZE_DIGITS + 1, ' ');
		if (error_code) {
			printf("Errro11\n");
		}
		printf("%s\n", asize);
		asize_n = atoi(asize);

		strcpy(path, "./");
		strcat(path, get_user_topic(user));
		strcat(path, "/");
		strcat(path, question);
		strcat(path, "_");
		strcat(path, answers);
		strcat(path, ".txt");
		printf("%s\n", path);

		write_from_socket_to_file(sock_tcp, path, asize_n);

		error_code = read_from_tcp_socket(sock_tcp, NULL, 0, ' ');
		if (error_code) {
			printf("Errro111\n");
		}
	
		error_code = read_from_tcp_socket(sock_tcp, &aimg, 1, '\0');
		if (error_code) {
			printf("Errro12\n");
		}

		if (aimg == '1') {

			error_code = read_from_tcp_socket(sock_tcp, NULL, 0, ' ');
			if (error_code) {
				printf("Errro122\n");
			}
			error_code = read_from_tcp_socket(sock_tcp, aiext, EXTENSION_SIZE_DIGITS + 1, ' ');
			if (error_code) {
				printf("Errro13\n");
			}

			error_code = read_from_tcp_socket(sock_tcp, aisize, FILE_SIZE_DIGITS + 1, ' ');
			if (error_code) {
				printf("Errro14\n");
			}

			aisize_n = atoi(aisize);

			strcpy(path, "./");
			strcat(path, get_user_topic(user));
			strcat(path, "/");
			strcat(path, question);
			strcat(path, "_");
			strcat(path, answers);
			strcat(path, ".");
			strcat(path, aiext);
			printf("%s - %d\n", path, aisize_n);
			write_from_socket_to_file(sock_tcp, path, aisize_n);
		}	
	}
	error_code = read_from_tcp_socket(sock_tcp, NULL, 0, '\n');
	if (error_code) {
		printf("Errro15\n");
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

static int recv_QUS_request(int sock_tcp, user_t *user, char *status) {
	int  error_code;
	char protocol[PROTOCOL_SIZE + 1];

	error_code = read_from_tcp_socket(sock_tcp, protocol, PROTOCOL_SIZE + 1, ' ');
	if (error_code) {
		return FAILURE;
	}
	error_code = read_from_tcp_socket(sock_tcp, protocol, STATUS_SIZE   + 1, ' ');
	if (error_code) {
		return FAILURE;
	}
	error_code = read_from_tcp_socket(sock_tcp, NULL, 0, '\n');
	if (error_code) {
		return FAILURE;
	}
	return SUCCESS;
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

static int recv_ANS_request(int sock_tcp, user_t *user, char *status) {
	int  error_code;
	char protocol[PROTOCOL_SIZE + 1];

	error_code = read_from_tcp_socket(sock_tcp, protocol, PROTOCOL_SIZE + 1, ' ');
	if (error_code) {
		return FAILURE;
	}
	error_code = read_from_tcp_socket(sock_tcp, protocol, STATUS_SIZE   + 1, ' ');
	if (error_code) {
		return FAILURE;
	}
	error_code = read_from_tcp_socket(sock_tcp, NULL, 0, '\n');
	if (error_code) {
		return FAILURE;
	}
	return SUCCESS;
}	

void client_tcp_manager(user_t *user, char* protocol, char args[MAX_ARGS_N][MAX_ARGS_L], int num_args) {
	int   error_code, server_sock_tcp;
	char *request, *size, *data, *img, *iext, *isize, *idata, status[STATUS_SIZE];;
	
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
			recv_GQU_request(user, args[1], server_sock_tcp);
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
						error_code = recv_QUS_request(server_sock_tcp, user, status);
						if (error_code == SUCCESS) {
							if (!strcmp(status, "OK")) {
								printf("%s\n", QUESTION_SUBMITION_SUCCEDED);
							}
							else if (!strcmp(status, "DUP")) {
								printf("%s\n", QUESTION_LIST_IS_FULL);
							}
							else if (!strcmp(status, "FUL")) {
								printf("%s\n", ANSWER_LIST_IS_FULL);
							}
							else {
								printf("%s\n", ANSWER_SUBMITION_FAILED);
							}
						}
					}
					else {
						isize = get_file_size(args[3], "r");
						iext  = get_file_ext(args[3]);
						if (isize && iext) {
							send_QUS_request(server_sock_tcp, protocol, get_user_id(user), get_user_topic(user), args[1], size, args[2], "1", iext, isize, args[3]);
							error_code = recv_ANS_request(server_sock_tcp, user, status);
							if (error_code == SUCCESS) {
								if (!strcmp(status, "OK")) {
									printf("%s\n", ANSWER_SUBMITION_SUCCEDED);
								}
								else if (!strcmp(status, "FUL")) {
									printf("%s\n", ANSWER_LIST_IS_FULL);
								}
								else {
									printf("%s\n", ANSWER_SUBMITION_FAILED);
								}
							}
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