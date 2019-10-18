#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "client-udp-manager.h"
#include "../entities/user.h"
#include "../constants.h"
#include "../exceptions.h"
#include "../entities/vector.h"
#include "../entities/question.h"
#include "../error-messages/input-error-messages.h"
#include "../error-messages/server-error-messages.h"

static char* make_new_REG_request(char *protocol, char *user_id) {
	char* request = (char *) malloc(sizeof(char) * (strlen(protocol) + strlen(user_id) + strlen(" ") + strlen("\n") + 1));
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
	char* request = (char *) malloc(sizeof(char) * (strlen(protocol) + strlen(protocol) + strlen("\n") + 1));
	if (request != NULL) {
		request[0] = '\0';
		strcpy(request, protocol);
		strcat(request, "\n\0");
	}
	return request;	
}

static char* make_new_PTP_request(char *protocol, char *user_id, char *topic) {
	char* request = (char *) malloc(sizeof(char) * (strlen(protocol) + strlen(user_id) + strlen(topic) + (strlen(" ") * 2) + strlen("\n") + 1));
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
	char* request = (char *) malloc(sizeof(char) * (strlen(protocol) + strlen(topic) + strlen(" ") + strlen("\n") + 1));
	if (request != NULL) {
		request[0] = '\0';
		strcpy(request, protocol);
		strcat(request, " ");
		strcat(request, topic);
		strcat(request, "\n\0");
	}
	return request;
}

static int parse_output_REG(char *response, char status[STATUS_SIZE]) {
	char protocol[PROTOCOL_SIZE + 1];
	int   i, j, n;

	n = strlen(response);

	if (n == 0 || response[n - 1] != '\n') {
		return BAD_OUTPUT;
	}

	for (j = 0, i = 0; i < PROTOCOL_SIZE && i < n - 1; i++, j++) {
		protocol[j] = response[i];
	}

	protocol[j] = '\0';

	if (strcmp(protocol, "RGR")) {
		return BAD_OUTPUT;
	}

	if (response[i] != ' ') {
		return BAD_OUTPUT;
	}

	for (j = 0, i = i + 1; i < n - 1 && j < STATUS_SIZE; i++, j++) {
		status[j] = response[i];
	}

	status[j] = '\0';

	if (response[i] != '\n' && response[i + 1] != '\0') {
		return BAD_OUTPUT;
	}

	return SUCCESS;
}

static int parse_output_LTP(char *response, user_t *user) {
	char     protocol[PROTOCOL_SIZE + 1], num_topics[MAX_TOPICS_NUMBER + 1], topic_name[TOPIC_TITLE_SIZE + 1], user_id[USER_ID_SIZE + 1];
	int      i, j, k, n, n_topics;
	topic_t  *topic;
	vector_t *topics;

	n = strlen(response);

	if (n == 0 || response[n - 1] != '\n') {
		return BAD_OUTPUT;
	}

	for (j = 0, i = 0; i < PROTOCOL_SIZE && i < n - 1; i++, j++) {
		protocol[j] = response[i];
	}

	protocol[j] = '\0';

	if (strcmp(protocol, "LTR")) {
		return BAD_OUTPUT;
	}

	if (response[i] != ' ') {
		return BAD_OUTPUT;
	}

	for (j = 0, i = i + 1; i < n - 1 && (response[i] >= '0' && response[i] <= '9'); i++, j++) {
		num_topics[j] = response[i];
	}

	num_topics[j] = '\0';
	n_topics      = atoi(num_topics);

	if (j == 0 || n_topics > 99) {
		return BAD_OUTPUT;
	}

	topics = vector_alloc(1, NULL);

	for (k = 0; k < n_topics; k++) {

		if (response[i] != ' ') {
			break;
		}

		for (j = 0, i = i + 1; i < n - 1 && (response[i] != ':') && j < TOPIC_TITLE_SIZE; i++, j++) {
			topic_name[j] = response[i];
		}

		topic_name[j] = '\0';

		if (response[i] != ':') {
			break;
		}

		for (j = 0, i = i + 1; i < n - 1 && (response[i] >= '0' && response[i] <= '9') && j < USER_ID_SIZE; i++, j++) {
			user_id[j] = response[i];
		}

		user_id[j] = '\0';

		if (j != 5) {
			break;
		}

		topic = create_topic(topic_name, user_id);
		vector_pushBack(topics, topic);
	}

	if (k != n_topics) {
		vector_free(topics);
		return BAD_OUTPUT;
	}

	set_user_topics(user, topics);

	vector_free(topics);

	if (response[i] != '\n' && response[i + 1] != '\0') {
		return BAD_OUTPUT;
	}

	return SUCCESS;
}

static int parse_input_PTP(char *response, char status[STATUS_SIZE]) {
	int  i, j, n;
	char protocol[PROTOCOL_SIZE + 1];

	n = strlen(response);

	if (n == 0 || response[n - 1] != '\n') {
		return BAD_OUTPUT;
	}

	for (j = 0, i = 0; i < PROTOCOL_SIZE && i < n - 1; i++, j++) {
		protocol[j] = response[i];
	}

	protocol[j] = '\0';

	if (strcmp(protocol, "PTR")) {
		return BAD_OUTPUT;
	}

	if (response[i] != ' ') {
		return BAD_OUTPUT;
	}

	for (j = 0, i = i + 1; i < n - 1 && (response[i] >= '0' && response[i] <= '9') && j < USER_ID_SIZE; i++, j++) {}
		
	if (j != USER_ID_SIZE) {
		return BAD_OUTPUT;
	}

	if (response[i] != ' ') {
		return BAD_OUTPUT;
	}

	for (i = i + 1; i < n - 1; i++) {}

	if (response[i] != '\n' && response[i + 1] != '\0') {
		return BAD_OUTPUT;
	}	

	return SUCCESS;
}

static int parse_output_PTP(char *response, char status[STATUS_SIZE]) {
	char protocol[PROTOCOL_SIZE + 1];
	int  i, j, n;

	n = strlen(response);

	if (n == 0 || response[n - 1] != '\n') {
		return BAD_OUTPUT;
	}

	for (j = 0, i = 0; i < PROTOCOL_SIZE && i < n - 1; i++, j++) {
		protocol[j] = response[i];
	}

	protocol[j] = '\0';

	if (strcmp(protocol, "PTR")) {
		return BAD_OUTPUT;
	}

	if (response[i] != ' ') {
		return BAD_OUTPUT;
	}

	for (j = 0, i = i + 1; i < n - 1 && j < STATUS_SIZE; i++, j++) {
		status[j] = response[i];
	}

	status[j] = '\0';

	if (response[i] != '\n' && response[i + 1] != '\0') {
		return BAD_OUTPUT;
	}
	
	return SUCCESS;
}


static int parse_output_LQU(char *response, user_t *user) {
	char         protocol[PROTOCOL_SIZE + 1], num_questions[MAX_QUESTIONS_NUMBER + 1], num_answers[MAX_ANSWERS_NUMBER], question_name[QUESTION_TITLE_SIZE + 1], user_id[USER_ID_SIZE + 1];
	int          i, j, k, n, n_questions, n_answers;
	question_t  *question;
	vector_t    *questions;

	n = strlen(response);

	if (n == 0 || response[n - 1] != '\n') {
		return BAD_OUTPUT;
	}

	for (j = 0, i = 0; i < PROTOCOL_SIZE && i < n - 1; i++, j++) {
		protocol[j] = response[i];
	}

	protocol[j] = '\0';

	if (strcmp(protocol, "LQR")) {
		return BAD_OUTPUT;
	}

	if (response[i] != ' ') {
		return BAD_OUTPUT;
	}

	for (j = 0, i = i + 1; i < n - 1 && (response[i] >= '0' && response[i] <= '9'); i++, j++) {
		num_questions[j] = response[i];
	}

	num_questions[j] = '\0';
	n_questions      = atoi(num_questions);

	if (j == 0 || n_questions > 99) {
		return BAD_OUTPUT;
	}

	questions = vector_alloc(1, NULL);

	for (k = 0; k < n_questions; k++) {
		if (response[i] != ' ') {
			break;
		}

		for (j = 0, i = i + 1; i < n - 1 && (response[i] != ':') && j < QUESTION_TITLE_SIZE; i++, j++) {
			question_name[j] = response[i];
		}

		question_name[j] = '\0';

		for (j = 0, i = i + 1; i < n - 1 && (response[i] >= '0' && response[i] <= '9') && j < USER_ID_SIZE; i++, j++) {
			user_id[j] = response[i];
		}

		user_id[j] = '\0';

		if (j != 5 || response[i] != ':') {
			break;
		}

		for (j = 0, i = i + 1; i < n - 1 && (response[i] >= '0' && response[i] <= '9'); i++, j++) {
			num_answers[j] = response[i];
		}

		num_answers[j] = '\0';
		n_answers      = atoi(num_answers);

		if (j == 0 || n_answers > 99) {
			return BAD_OUTPUT;
		}

		question = create_question(question_name, user_id, num_answers);
		vector_pushBack(questions, question);
	}

	if (k != n_questions) {
		vector_free(questions);
		return BAD_OUTPUT;
	}

	set_user_questions(user, questions);

	vector_free(questions);

	if (response[i] != '\n' && response[i + 1] != '\0') {
		return BAD_OUTPUT;
	}

	return SUCCESS;
}

static char* send_request(user_t *user, char *request) {
	char 			  *buffer;
	struct sockaddr_in addr;
	ssize_t 		   n;
	socklen_t 		   addrlen = sizeof(addr);

	buffer = (char *) malloc (sizeof(char) * (BUFFER_SIZE + 1));

	if (buffer != NULL) {
		n = sendto(get_user_server_sock_udp(user), request, strlen(request), 0, (struct sockaddr *) (get_user_udp_addrinfo(user)->ai_addr), (get_user_udp_addrinfo(user)->ai_addrlen));
		if (n == -1) {
			fprintf(stderr, "sendto failed: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
			
		n = recvfrom(get_user_server_sock_udp(user), buffer, BUFFER_SIZE, 0, (struct sockaddr *) &addr, &addrlen);
		if (n == -1) {
			fprintf(stderr, "recvfrom failed: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		buffer[n] = '\0';
	}
	return buffer;
}

void
client_udp_manager(user_t *user, char* protocol, char args[MAX_ARGS_N][MAX_ARGS_L]) {
	char  status[STATUS_SIZE], *request, *response;
	int   error_code, i;

	if (!strcmp(protocol, "REG")) {
		request = make_new_REG_request(protocol, args[1]);
		if (request != NULL) {
			response = send_request(user, request);
			if (response) {
				error_code = parse_output_REG(response, status);
				if (error_code == SUCCESS) {
					if (!strcmp(status, "OK\0")) {
						printf("%s\n", REGISTRATION_SUCCESSFUL);
 						set_user_id(user, args[1]);
 					}
 					else {
 						printf("%s\n", REGISTRATION_UNSUCCESSFUL);
 					}
				}

				free(response);
				response = NULL;
			}
			
			free(request);
			request = NULL;
		}
	}

	else if (!strcmp(protocol, "LTP")) {
		request = make_new_LTP_request(protocol);	
		if (request != NULL) {
			response = send_request(user, request);
			if (response) {
				error_code = parse_output_LTP(response, user);
				if (error_code == SUCCESS) {
					for (i = 0; i < user->topics->size; i++) {
						printf("Topic %2d: %-10s by user: %s\n", 
							i + 1,
							((topic_t*) vector_at(user->topics, i))->topic_name, 
							((topic_t*) vector_at(user->topics, i))->topic_user);
					}
					if (i == 0) {
						printf("No topics to print\n");
					}
				}
				free(response);
				response = NULL;
			}
			
			free(request);
			request = NULL;
		}
	}

	else if (!strcmp(protocol, "PTP")) {
		if (get_user_id(user)) {
			request = make_new_PTP_request(protocol, get_user_id(user), args[1]);
			if (request != NULL) {
				response = send_request(user, request);
				if (response) {
					error_code = parse_output_PTP(response, status);
					if (error_code == SUCCESS) {
						if (!strcmp(status, "OK")) {
							printf("Topic %s accepted!\n", args[1]);
						}
						else if (!strcmp(status, "DUP")) {
							printf("Topic %s already exists!\n", args[1]);
						}
						else if (!strcmp(status, "FULL")) {
							printf("Max topics reached out!\n");
						}
						else if (!strcmp(status, "NOK")) {
							printf("Topic %s not accepted!\n", args[1]);
						}
					}

					free(response);
					response = NULL;
				}
				
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
				response = send_request(user, request);
				if (response) {
					error_code = parse_output_LQU(response, user);
					if (error_code == SUCCESS) {
						for (i = 0; i < user->questions->size; i++) {
							printf("Question %2d: %-10s by user: %s width %d answers\n", 
								i + 1,
								((question_t*) vector_at(user->questions, i))->question_name, 
								((question_t*) vector_at(user->questions, i))->question_user,
								atoi(((question_t*) vector_at(user->questions, i))->answers_number));
						}
						if (i == 0) {
							printf("No questions available for the selected topic\n");
						}
					}

					free(response);
					response = NULL;
				}
				
				free(request);
				request = NULL;
			}
		}
		else {
			printf("%s\n", TOPIC_IS_NOT_SELECTED);
		}
	}
}