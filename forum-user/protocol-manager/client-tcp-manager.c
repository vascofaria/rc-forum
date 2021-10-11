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

static int send_GQU_request(int sock_tcp, char *protocol, char *topic, char *question) {
	int error_code;
	error_code = write_to_tcp_socket(sock_tcp, protocol, ' ');
	if (error_code) {
		return error_code;
	}
	error_code = write_to_tcp_socket(sock_tcp, topic, ' ');
	if (error_code) {
		return error_code;
	}
	error_code = write_to_tcp_socket(sock_tcp, question, '\n');
	if (error_code) {
		return error_code;
	}
	return SUCCESS;
}

static int recv_GQU_request(user_t *user, char *question, int sock_tcp) {
	char path[MAX_PATH_SIZE];
	char protocol[PROTOCOL_SIZE + 1];
	char q_user_id[USER_ID_SIZE + 1], qsize[FILE_SIZE_DIGITS + 1], qiext[EXTENSION_SIZE_DIGITS + 1], qisize[FILE_SIZE_DIGITS + 1], answers[MAX_ANSWERS_SIZE_DIGITS + 1], qimg, c;
	char a_user_id[USER_ID_SIZE + 1], asize[FILE_SIZE_DIGITS + 1], aiext[EXTENSION_SIZE_DIGITS + 1], aisize[FILE_SIZE_DIGITS + 1], aimg;
	int  qsize_n, qisize_n, asize_n, aisize_n, answers_n, i, error_code, file_fd, ans_counter;

	if (access(get_user_topic(user), F_OK) != 0) {
		/* Create Topic directory */
		error_code = mkdir(get_user_topic(user), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		if (error_code) {
			return FAILURE;
		}	
	}

	error_code = read_from_tcp_socket(sock_tcp, protocol, PROTOCOL_SIZE + 1, ' ');
	if (error_code) {
		return error_code;
	}

	if (strcmp(protocol, "QGR")) {
		return BAD_INPUT;
	}

	error_code = read_from_tcp_socket(sock_tcp, q_user_id, USER_ID_SIZE + 1, ' ');
	if (error_code) {
		return error_code;
	}

	if (strlen(q_user_id) != USER_ID_SIZE) {
		return BAD_INPUT;
	}

	for (i = 0; i < USER_ID_SIZE; i++) {
		if (q_user_id[i] < '0' || q_user_id[i] > '9') {
			return BAD_INPUT;
		}
	}

	printf("Question %s by user %s - files downloaded\n", question, q_user_id);

	error_code =  read_from_tcp_socket(sock_tcp, qsize, FILE_SIZE_DIGITS + 1, ' ');
	if (error_code) {
		return FAILURE;
	}

	for (i = 0; i < strlen(qsize); i++) {
		if (qsize[i] < '0' || qsize[i] > '9') {
			return BAD_INPUT;
		}
	}

	qsize_n = atoi(qsize);

	strcpy(path, "./");
	strcat(path, get_user_topic(user));
	strcat(path, "/");
	strcat(path, question);
	strcat(path, ".txt\0");

	error_code = write_from_socket_to_file(sock_tcp, path, qsize_n);
	if (error_code) {
		return error_code;
	}
	
	error_code =  read_from_tcp_socket(sock_tcp, NULL, 0, ' ');
	if (error_code) {
		return error_code;
	}

	error_code = read_from_tcp_socket(sock_tcp, &qimg, 1, '\0');
	if (error_code) {
		return error_code;
	}


	error_code = read_from_tcp_socket(sock_tcp, NULL, 0, ' ');
	if (error_code) {
		return error_code;
	}


	if (qimg == '1') {

		error_code = read_from_tcp_socket(sock_tcp, qiext, EXTENSION_SIZE_DIGITS + 1, ' ');
		if (error_code) {
			return error_code;
		}

		error_code = read_from_tcp_socket(sock_tcp, qisize, FILE_SIZE_DIGITS + 1, ' ');
		if (error_code) {
			return error_code;
		}

		for (i = 0; i < strlen(qisize); i++) {
			if (qisize[i] < '0' || qisize[i] > '9') {
				return BAD_INPUT;
			}
		}

		qisize_n = atoi(qisize);
		
		strcpy(path, "./");
		strcat(path, get_user_topic(user));
		strcat(path, "/");
		strcat(path, question);
		strcat(path, ".");
		strcat(path, qiext);
		
		error_code = write_from_socket_to_file(sock_tcp, path, qisize_n);
		if (error_code) {
			return error_code;
		}


		error_code = read_from_tcp_socket(sock_tcp, NULL, 0, ' ');
		if (error_code) {
			return error_code;
		}

	} else if (qimg != '0') {
		return BAD_INPUT;
	}

	error_code = read_from_tcp_socket(sock_tcp, answers, MAX_ANSWERS_SIZE_DIGITS, '\0');
	if (error_code) {
		return error_code;
	}
	if (strlen(answers) != MAX_ANSWERS_SIZE_DIGITS) {
		return BAD_INPUT;
	}
	for (i = 0; i < MAX_ANSWERS_SIZE_DIGITS; i++) {
		if (answers[i] < '0' || answers[i] > '9') {
			return BAD_INPUT;
		}
	}
	answers[MAX_ANSWERS_SIZE_DIGITS] = '\0';
	answers_n = atoi(answers);

	printf("%s answers found for this question: \n", answers);

	for (ans_counter = 0; ans_counter < answers_n; ans_counter++) {
		error_code = read_from_tcp_socket(sock_tcp, NULL, 0, ' ');
		if (error_code) {
			return error_code;
		}

		error_code = read_from_tcp_socket(sock_tcp, answers, MAX_ANSWERS_SIZE_DIGITS + 1, ' ');
		if (error_code) {
			return error_code;
		}
		if (strlen(answers) != MAX_ANSWERS_SIZE_DIGITS) {
			return BAD_INPUT;
		}
		for (i = 0; i < MAX_ANSWERS_SIZE_DIGITS; i++) {
			if (answers[i] < '0' || answers[i] > '9') {
				return BAD_INPUT;
			}
		}

		error_code =  read_from_tcp_socket(sock_tcp, a_user_id, USER_ID_SIZE + 1, ' ');
		if (error_code) {
			return error_code;
		}
		if (strlen(a_user_id) != USER_ID_SIZE) {
			return BAD_INPUT;
		}
		for (i = 0; i < USER_ID_SIZE; i++) {
			if (a_user_id[i] < '0' || a_user_id[i] > '9') {
				return BAD_INPUT;
			}
		}

		printf("\nRetrieving answer %s_%s by user %s\n", question, answers, a_user_id);

		error_code = read_from_tcp_socket(sock_tcp, asize, FILE_SIZE_DIGITS + 1, ' ');
		if (error_code) {
			return error_code;
		}
		for (i = 0; i < strlen(asize); i++) {
			if (asize[i] < '0' || asize[i] > '9') {
				return BAD_INPUT;
			}
		}
		asize_n = atoi(asize);

		strcpy(path, "./");
		strcat(path, get_user_topic(user));
		strcat(path, "/");
		strcat(path, question);
		strcat(path, "_");
		strcat(path, answers);
		strcat(path, ".txt");

		error_code = write_from_socket_to_file(sock_tcp, path, asize_n);
		if (error_code) {
			return error_code;
		}

		error_code = read_from_tcp_socket(sock_tcp, NULL, 0, ' ');
		if (error_code) {
			return error_code;
		}

		error_code = read_from_tcp_socket(sock_tcp, &aimg, 1, '\0');
		if (error_code) {
			return error_code;
		}

		if (aimg == '1') {

			printf("Answer %s_%s contains an image\n", question, answers);

			error_code = read_from_tcp_socket(sock_tcp, NULL, 0, ' ');
			if (error_code) {
				return error_code;
			}
			error_code = read_from_tcp_socket(sock_tcp, aiext, EXTENSION_SIZE_DIGITS + 1, ' ');
			if (error_code) {
				return error_code;
			}
			error_code = read_from_tcp_socket(sock_tcp, aisize, FILE_SIZE_DIGITS + 1, ' ');
			if (error_code) {
				return error_code;
			}
			for (i = 0; i < strlen(aisize); i++) {
				if (aisize[i] < '0' || aisize[i] > '9') {
					return BAD_INPUT;
				}
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

			error_code = write_from_socket_to_file(sock_tcp, path, aisize_n);
			if (error_code) {
				return error_code;
			}

		} else if (aimg != '0') {
			return BAD_INPUT;
		}
	}
	error_code = read_from_tcp_socket(sock_tcp, NULL, 0, '\n');
	if (error_code) {
		return error_code;
	}

	printf("Operation successful. Check files\n");

	return SUCCESS;
}

static int send_QUS_request(int sock_tcp, char *protocol, char* user_id, char *topic, char *question, char *qsize, char *qpath, char *qimg, char *iext, char *isize, char *ipath) {
	int error_code;

	error_code = write_to_tcp_socket(sock_tcp, protocol, ' ');
	if (error_code) {
		return error_code;
	}
	error_code = write_to_tcp_socket(sock_tcp, user_id, ' ');
	if (error_code) {
		return error_code;
	}
	error_code = write_to_tcp_socket(sock_tcp, topic, ' ');
	if (error_code) {
		return error_code;
	}
	error_code = write_to_tcp_socket(sock_tcp, question, ' ');
	if (error_code) {
		return error_code;
	}
	error_code = write_to_tcp_socket(sock_tcp, qsize, ' ');
	if (error_code) {
		return error_code;
	}
	error_code = write_from_file_to_socket(sock_tcp, qpath, atoi(qsize));
	if (error_code) {
		return error_code;
	}
	error_code = write_to_tcp_socket(sock_tcp, "\0", ' ');
	if (error_code) {
		return error_code;
	}
	error_code = write_to_tcp_socket(sock_tcp, qimg, '\0');
	if (error_code) {
		return error_code;
	}
	if (!strcmp(qimg, "1")) {
		error_code = write_to_tcp_socket(sock_tcp, "\0", ' ');
		if (error_code) {
			return error_code;
		}
		error_code = write_to_tcp_socket(sock_tcp, iext, ' ');
		if (error_code) {
			return error_code;
		}
		error_code = write_to_tcp_socket(sock_tcp, isize, ' ');
		if (error_code) {
			return error_code;
		}
		error_code = write_from_file_to_socket(sock_tcp, ipath, atoi(isize));
		if (error_code) {
			return error_code;
		}
	}
	error_code = write_to_tcp_socket(sock_tcp, "\0", '\n');
	if (error_code) {
		return error_code;
	}
	return SUCCESS;
}

static int recv_QUS_request(int sock_tcp, user_t *user, char *status) {
	int  error_code;
	char protocol[PROTOCOL_SIZE + 1];

	error_code = read_from_tcp_socket(sock_tcp, protocol, PROTOCOL_SIZE + 1, ' ');
	if (error_code) {
		return error_code;
	}

	if (strcmp(protocol, "QUR")) {
		return BAD_INPUT;
	}

	error_code = read_from_tcp_socket(sock_tcp, status, STATUS_SIZE, '\n');
	if (error_code) {
		return error_code;
	}
	return SUCCESS;
}

static int send_ANS_request(int sock_tcp, char *protocol, char* user_id, char *topic, char *question, char *asize, char *apath, char *aimg, char *iext, char *isize, char *ipath) {
	int error_code;

	error_code = write_to_tcp_socket(sock_tcp, protocol, ' ');
	if (error_code) {
		return error_code;
	}
	error_code = write_to_tcp_socket(sock_tcp, user_id, ' ');
	if (error_code) {
		return error_code;
	}
	error_code = write_to_tcp_socket(sock_tcp, topic, ' ');
	if (error_code) {
		return error_code;
	}
	error_code = write_to_tcp_socket(sock_tcp, question, ' ');
	if (error_code) {
		return error_code;
	}
	error_code = write_to_tcp_socket(sock_tcp, asize, ' ');
	if (error_code) {
		return error_code;
	}
	error_code = write_from_file_to_socket(sock_tcp, apath, atoi(asize));
	if (error_code) {
		return error_code;
	}
	error_code = write_to_tcp_socket(sock_tcp, "\0", ' ');
	if (error_code) {
		return error_code;
	}
	error_code = write_to_tcp_socket(sock_tcp, aimg, '\0');
	if (error_code) {
		return error_code;
	}
	if (!strcmp(aimg, "1")) {
		error_code = write_to_tcp_socket(sock_tcp, "\0", ' ');
		if (error_code) {
			return error_code;
		}
		error_code = write_to_tcp_socket(sock_tcp, iext, ' ');
		if (error_code) {
			return error_code;
		}
		error_code = write_to_tcp_socket(sock_tcp, isize, ' ');
		if (error_code) {
			return error_code;
		}
		error_code = write_from_file_to_socket(sock_tcp, ipath, atoi(isize));
		if (error_code) {
			return error_code;
		}
	} else if (strcmp(aimg, "0")) {
		return BAD_INPUT;
	}
	error_code = write_to_tcp_socket(sock_tcp, "\0", '\n');
	if (error_code) {
		return error_code;
	}

	return SUCCESS;
}

static int recv_ANS_request(int sock_tcp, user_t *user, char *status) {
	int  error_code;
	char protocol[PROTOCOL_SIZE + 1];

	error_code = read_from_tcp_socket(sock_tcp, protocol, PROTOCOL_SIZE + 1, ' ');
	if (error_code) {
		return error_code;
	}

	if (strcmp(protocol, "ANR")) {
		return BAD_INPUT;
	}

	error_code = read_from_tcp_socket(sock_tcp, status, STATUS_SIZE + 1, '\n');
	if (error_code) {
		return error_code;
	}

	return SUCCESS;
}	

void client_tcp_manager(user_t *user, char* protocol, char args[MAX_ARGS_N][MAX_ARGS_L], int num_args) {
	int   error_code, server_sock_tcp;
	char *request, *size, *data, *img, *iext, *isize, *idata, *status;
	
	status = (char *) malloc (sizeof(char) * (STATUS_SIZE + 1));
	if (status == NULL) {
		fprintf(stderr, "memory allocation failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

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

			error_code = send_GQU_request(server_sock_tcp, protocol, get_user_topic(user), args[1]);
			if (error_code) {
				if (error_code == BAD_INPUT) {
					printf("%s\n", SERVER_RESPONSE_ERROR);
				} else {
					printf("%s\n", SYSTEM_ERROR);
				}
			}

			error_code = recv_GQU_request(user, args[1], server_sock_tcp);
			if (error_code) {
				if (error_code == BAD_INPUT) {
					printf("%s\n", SERVER_RESPONSE_ERROR);
				} else {
					printf("%s\n", SYSTEM_ERROR);
				}
			}
		}	else {
			printf("%s\n", TOPIC_IS_NOT_SELECTED);
		}
	
	}	else if (!strcmp(protocol, "QUS")) {
		if (get_user_id(user)) {
			if (get_user_topic(user)) {	
				size = get_file_size(args[2], "r");
				if (size) {
					if (num_args == 3) {
						error_code = send_QUS_request(server_sock_tcp, protocol, get_user_id(user), get_user_topic(user), args[1], size, args[2], "0", "", "", "");
						if (error_code) {
							if (error_code == BAD_INPUT) {
								printf("%s\n", SERVER_RESPONSE_ERROR);
							} else {
								printf("%s\n", SYSTEM_ERROR);
							}
						}

					}	else {
						isize = get_file_size(args[3], "r");
						iext  = get_file_ext(args[3]);
						if (isize && iext) {
							error_code = send_QUS_request(server_sock_tcp, protocol, get_user_id(user), get_user_topic(user), args[1], size, args[2], "1", iext, isize, args[3]);
							if (error_code) {
								if (error_code == BAD_INPUT) {
									printf("%s\n", SERVER_RESPONSE_ERROR);
								} else {
									printf("%s\n", SYSTEM_ERROR);
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

					error_code = recv_QUS_request(server_sock_tcp, user, status);
					if (error_code == SUCCESS) {
						if (!strcmp(status, "OK")) {
								printf("%s\n", QUESTION_SUBMITION_SUCCEDED);
						}
						else if (!strcmp(status, "DUP")) {
								printf("%s\n", QUESTION_ALREADY_EXISTS);
						}
						else if (!strcmp(status, "FUL")) {
								printf("%s\n", QUESTION_LIST_IS_FULL);
						}
						else {
							printf("%s\n", QUESTION_SUBMITION_FAILED);
						}
					} else {
						if (error_code == BAD_INPUT) {
							printf("%s\n", SERVER_RESPONSE_ERROR);
						}
						else {
							printf("%s\n", SYSTEM_ERROR);
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
							error_code = send_ANS_request(server_sock_tcp, protocol, get_user_id(user), get_user_topic(user), get_user_question(user), size, args[1], "0", "", "", "");
							if (error_code) {
								if (error_code == BAD_INPUT) {
									printf("%s\n", SERVER_RESPONSE_ERROR);
								} else {
									printf("%s\n", SYSTEM_ERROR);
								}
							}
						}
						else {
							isize = get_file_size(args[2], "r");
							iext  = get_file_ext(args[2]);

							if (isize && iext) {
								error_code = send_ANS_request(server_sock_tcp, protocol, get_user_id(user), get_user_topic(user), get_user_question(user), size, args[1], "1", iext, isize, args[2]);
								if (error_code) {
									if (error_code == BAD_INPUT) {
										printf("%s\n", SERVER_RESPONSE_ERROR);
									} else {
										printf("%s\n", SYSTEM_ERROR);
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
						} else {
							if (error_code == BAD_INPUT) {
								printf("%s\n", SERVER_RESPONSE_ERROR);
							} else {
								printf("%s\n", SYSTEM_ERROR);
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

	free(status);
}
