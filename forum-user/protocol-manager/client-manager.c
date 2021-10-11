#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>


#include "client-manager.h"
#include "client-udp-manager.h"
#include "client-tcp-manager.h"
#include "../entities/user.h"
#include "../constants.h"
#include "../exceptions.h"
#include "../entities/topic.h"
#include "../entities/question.h"
#include "../error-messages/input-error-messages.h"

int count_white_spaces(char *buffer) {
	int i, count = 0;

	for (int i = 0; i < strlen(buffer); i++) {
		if (isspace(buffer[i])) {
			count++;
		}
	}

	return count;
}

int verify_number(char *buffer) {
	int i;
	for (i = 0; i < strlen(buffer); i++) {
		if (buffer[i] < '0' || buffer[i] > '9') {
			printf("%s\n", BAD_NUMBER);
			return FAILURE;
		}
	}
	return SUCCESS;
}

int verify_user_id(char *buffer) {
	int i;
	for (i = 0; i < strlen(buffer); i++) {
		if (buffer[i] < '0' || buffer[i] > '9') {
			printf("%s\n", BAD_USER_ID);
			return FAILURE;
		}
	}
	if (i != USER_ID_SIZE) {
		printf("%s\n", BAD_USER_ID);
		return FAILURE;
	}
	return SUCCESS;
}

int verify_topic(char *buffer) {
	if (strlen(buffer) > TOPIC_TITLE_SIZE) {
		printf("%s\n", BAD_TOPIC_TITLE);
		return FAILURE;
	}
	return SUCCESS;
}

int verify_question(char *buffer) {
	if (strlen(buffer) > QUESTION_TITLE_SIZE) {
		printf("%s\n", BAD_QUESTION_TITLE);
		return FAILURE;
	}
	return SUCCESS;
}

void
client_manager(user_t *user, char *request) {
	char       args[MAX_ARGS_N][MAX_ARGS_L];
	int        num_tokens, error_code;
	topic_t    *topic;
	question_t *question;

	num_tokens = sscanf(request, "%s %s %s %s %s", args[0], args[1], args[2], args[3], args[4]);

	if (!strcmp(args[0], "register")             || 
		!strcmp(args[0], "reg")) {
			if (count_white_spaces(request) - 1 != 1) {
				fprintf(stderr, "%s\n", REQUEST_IS_NOT_CORRECTLY_FORMULATED);
			}
			else if (num_tokens == 2) {
				if (verify_user_id(args[1]) == FAILURE) {
					return;
				}
				client_udp_manager(user, "REG", args);
			}
			else {
				fprintf(stderr, "Invalid number of parameters: %d\n", num_tokens);
			}
	}
	else if (!strcmp(args[0], "topic_list")      || 
		     !strcmp(args[0], "tl")) {
			if (count_white_spaces(request) - 1 != 0) {
				fprintf(stderr, "%s\n", REQUEST_IS_NOT_CORRECTLY_FORMULATED);
			}
			else if (num_tokens == 1) {
				client_udp_manager(user, "LTP", args);
			}
			else {
				fprintf(stderr, "Invalid number of parameters: %d\n", num_tokens);
			}
	}
	else if (!strcmp(args[0], "topic_select")) {
			if (count_white_spaces(request) - 1 != 1) {
				fprintf(stderr, "%s\n", REQUEST_IS_NOT_CORRECTLY_FORMULATED);
			}
			else if (num_tokens != 2) {
				fprintf(stderr, "Invalid number of parameters: %d\n", num_tokens);
			}
			else {
				if (verify_topic(args[1]) == FAILURE) {
					return;
				}
				set_user_topic(user, args[1]);
			} 
	}
	else if (!strcmp(args[0], "ts")) {
			if (count_white_spaces(request) - 1 != 1) {
				fprintf(stderr, "%s\n", REQUEST_IS_NOT_CORRECTLY_FORMULATED);
			}
			else if (num_tokens != 2) {
				fprintf(stderr, "Invalid number of parameters: %d\n", num_tokens);
			}
			else {
				if (verify_number(args[1]) == FAILURE) {
					return;
				}
				if (get_user_topics(user)->size > 0) {
					topic = get_topic_from_topiclist(user, atoi(args[1]));
					if (topic) {
						set_user_topic(user, get_topic_name(topic));
						printf("Topic %s selected!\n", get_topic_name(topic));
					}
				}
				else {
					printf("Topic list not available\n");
				}
			} 
	}
	else if (!strcmp(args[0], "topic_propose")   || 
		     !strcmp(args[0], "tp")) {
			if (count_white_spaces(request) - 1 != 1) {
				fprintf(stderr, "%s\n", REQUEST_IS_NOT_CORRECTLY_FORMULATED);
			}
			else if (num_tokens == 2) {
				if (verify_topic(args[1]) == FAILURE) {
					return;
				}
				client_udp_manager(user, "PTP", args);
			}
			else {
				fprintf(stderr, "Invalid number of parameters: %d\n", num_tokens);
			}
	}
	else if (!strcmp(args[0], "question_list")   || 
		     !strcmp(args[0], "ql")) {
			if (count_white_spaces(request) - 1 != 0) {
				fprintf(stderr, "%s\n", REQUEST_IS_NOT_CORRECTLY_FORMULATED);
			}
			else if (num_tokens == 1) {
				client_udp_manager(user, "LQU", args);
			}
			else {
				fprintf(stderr, "Invalid number of parameters: %d\n", num_tokens);
			}
	}
	else if (!strcmp(args[0], "question_get")    || 
		     !strcmp(args[0], "qg")) {
			if (count_white_spaces(request) - 1 != 1) {
				fprintf(stderr, "%s\n", REQUEST_IS_NOT_CORRECTLY_FORMULATED);
			}
			else if (num_tokens == 2) {
				if (!strcmp(args[0], "qg")) {
					if (verify_number(args[1]) == FAILURE) {
						return;
					}
					if (get_user_questions(user)->size > 0) {
						question = get_question_from_questionlist(user, atoi(args[1]));
						if (question) {
							strcpy(args[1], get_question_name(question));
							printf("%s\n", args[1]);
						}
					}
					else {
						printf("Question list not available\n");
						return;
					}	
				}
				else {
					if (verify_question(args[1]) == FAILURE) {
						return;
					}
				}

				client_tcp_manager(user, "GQU", args, num_tokens);
			}
			else {
				fprintf(stderr, "Invalid number of parameters: %d\n", num_tokens);
			}
	}
	else if (!strcmp(args[0], "question_submit") || 
		     !strcmp(args[0], "qs")) {
			if ((num_tokens == 3 && count_white_spaces(request) - 1 == 2) || (num_tokens == 4 && count_white_spaces(request) - 1 == 3)) { 
				if (verify_question(args[1]) == FAILURE) {
					return;
				}
				client_tcp_manager(user, "QUS", args, num_tokens);
			}
			else if ((num_tokens == 3 && count_white_spaces(request) - 1 != 2) || (num_tokens == 4 && count_white_spaces(request) - 1 != 3)) { 
				printf("%s\n", REQUEST_IS_NOT_CORRECTLY_FORMULATED);
			}
			else {
				fprintf(stderr, "Invalid number of parameters: %d\n", num_tokens);
			}
	}
	else if (!strcmp(args[0], "answer_submit")   || 
		     !strcmp(args[0], "as")) {
			if (num_tokens == 2 && count_white_spaces(request) - 1 == 1 || num_tokens == 3 && count_white_spaces(request) - 1 == 2) {	
				client_tcp_manager(user, "ANS", args, num_tokens);
			}
			else if (num_tokens == 2 && count_white_spaces(request) - 1 != 1 || num_tokens == 3 && count_white_spaces(request) - 1 != 2) {	
				printf("%s\n", REQUEST_IS_NOT_CORRECTLY_FORMULATED);
			}
			else {
				fprintf(stderr, "Invalid number of parameters: %d\n", num_tokens);
			}
	}
	else if (!strcmp(args[0], "?")) {
			if (count_white_spaces(request) - 1 != 0) {
				fprintf(stderr, "%s\n", REQUEST_IS_NOT_CORRECTLY_FORMULATED);
			}
			else if (num_tokens == 1) {
				printf( "\tregister\t/reg\t<userId>\n" 
						"\ttopic_list\t/tl\n"
						"\ttopic_select\t\t<topic name>\n"
						"\tts\t\t\t<topic number>\n"
						"\ttopic_propose\t/tp\t<topic>\n"
						"\tquestion_list\t/ql\n"
						"\tquestion_get\t\t<question name>\n"
						"\tqg\t\t\t<question_number>\n"
						"\tanswer_submit\t/as\t<text_file [image_file]>\n"
						"\tquestion_submit\t/qs\t<text_file [image_file]>\n"
						"\texit\n");
			}
			else {
				fprintf(stderr, "Invalid number of parameters: %d\n", num_tokens);
			}
	}
	else if (!strcmp(args[0], "exit")) {
			if (count_white_spaces(request) - 1 != 0) {
				fprintf(stderr, "%s\n", REQUEST_IS_NOT_CORRECTLY_FORMULATED);
			}
			else if (num_tokens == 1) {
				error_code = close(get_user_server_sock_udp(user));
				if (error_code == -1) {
					fprintf(stderr, "close failed: %s\n", strerror(errno));
					exit(EXIT_FAILURE);
				}
				freeaddrinfo(get_user_udp_addrinfo(user));
				freeaddrinfo(get_user_tcp_addrinfo(user));
				exit(EXIT_SUCCESS);
			}
			else {
				fprintf(stderr, "Invalid number of parameters: %d\n", num_tokens);
			}
	}
	else {
			fprintf(stdout, "Invalid command!\n");
	}
}