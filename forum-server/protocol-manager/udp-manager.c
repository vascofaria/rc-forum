#include "udp-manager.h"
#include "protocol.h"
#include "../exceptions.h"
#include "../constants.h"
#include "../usecases/usecases.h"
#include <stdio.h>
#include <string.h>


static int parse_input_REG(char* request, char* user_id) {
	int i, j;

	if (request[PROTOCOL_SIZE] != ' ') {
		return BAD_INPUT;
	}

	for (i = PROTOCOL_SIZE + 1, j = 0; j < USER_ID_SIZE && request[i] != '\0' && request[i] != '\n'; i++, j++) {
		if (request[i] < '0' || request[i] > '9') {
			return BAD_INPUT;
		}	
		user_id[j] = request[i];
	}

	if (j != USER_ID_SIZE) {
		return BAD_INPUT;
	}

	user_id[j] = '\0';

	if (request[i] != '\n' || request[i+1] != '\0') {
		return BAD_INPUT;
	}

	return SUCCESS;
}

static char *parse_output_RGR() {
	char *response = (char*) malloc(sizeof(char) * (MAX_STATUS_RESPONSE+1));
	strcpy(response, RGR);
	strcat(response, OK);
	return response;
}

static int parse_input_LTP(char* request) {

	if (request[PROTOCOL_SIZE] != '\n' || request[PROTOCOL_SIZE+1] != '\0') {
		return BAD_INPUT;
	}

	return SUCCESS;
}

static char *parse_output_LTR(char **topics_list) {
	int  num;
	char num_str[3];
	char topics[MAX_TOPIC_LIST_RESPONSE] = "\0";
	char *response = (char*) malloc(sizeof(char)*(MAX_TOPIC_LIST_RESPONSE + PROTOCOL_SIZE + 3));

	for(num = 0; num < MAX_TOPIC_LIST_RESPONSE && topics_list[num] != NULL; num++) {
	    strcat(topics, " ");
	    strcat(topics, topics_list[num]);
	}

	sprintf(num_str, "%d", num);

	strcat(response, LTR);
	strcat(response, " ");
	strcat(response, num_str);

	if (num == 0) strcat(response, '\n');
	else strcat(response, topics);
	
	return response;
}

static int parse_input_PTP(char* request, char* user_id, char* topic) {
	int i, j;

	if (request[PROTOCOL_SIZE] != ' ') {
		return BAD_INPUT;
	}

	for (i = PROTOCOL_SIZE + 1, j = 0; j < USER_ID_SIZE && request[i] != '\0' && request[i] != '\n'; i++, j++) {
		if (request[i] < '0' || request[i] > '9') {
			return BAD_INPUT;
		}
		user_id[j] = request[i];
	}
	user_id[j] = '\0';

	if (request[i++] != ' ') {
		return BAD_INPUT;
	}

	for (j = 0; j < TOPIC_SIZE && request[i] != '\0' && request[i] != '\n'; i++, j++) {
		if (request[i] == ' ') {
			return BAD_INPUT;
		}
		topic[j] = request[i];
	}
	topic[j] = '\0';

	return SUCCESS;
}

static char *parse_output_PTR() {

	char *response = (char*) malloc(sizeof(char)*(MAX_STATUS_RESPONSE+1));
	strcat(response, PTR);
	strcat(response, OK);

	return response;
}

static int parse_input_LQU(char* request, char* topic) {
	int i, j;

	if (request[PROTOCOL_SIZE] != ' ') {
		return BAD_INPUT;
	}

	for (i = PROTOCOL_SIZE + 1, j = 0; j < TOPIC_SIZE && request[i] != '\0' && request[i] != '\n'; i++, j++) {
		if (request[i] == ' ') {
			return BAD_INPUT;
		}
		topic[j] = request[i];
	}
	topic[j] = '\0';

	return SUCCESS;
}

static char *parse_output_LQR(char* topic, char **questions_list){
	int num = 0;
	
	char questions[MAX_TOPIC_LIST_RESPONSE];
	char *response = (char*) malloc(sizeof(char)*(MAX_TOPIC_LIST_RESPONSE + 6));
	
	for(num = 0; num < MAX_TOPIC_LIST_RESPONSE && questions_list[num] != NULL; num++){
		strcat(questions, " ");
		strcat(questions, questions_list[num]);
	}
	
	strcat(response, LQR);
	strcat(response, " ");
	strcat(response, num);
	strcat(response, questions);
	return response;
}

/*
 * ERROR PROTOCOL
*/
static char *parse_output_ERROR(int error_code) {
	
	char* response = (char*) malloc(sizeof(char)*(MAX_STATUS_RESPONSE+1));
	if (error_code == BAD_INPUT) {
		strcpy(response, "BAD_INPUT\0");
		return response;
	} else if (error_code == TOPIC_DOESNT_EXIST) {
		strcpy(response, "TOPIC_DOESNT_EXIST\0");
		return response;
	} else if (error_code == TOPIC_ALREADY_EXISTS) {
		strcpy(response, "TOPIC_ALREADY_EXISTS\0");
		return response;
	} else if (error_code == QUESTION_DOESNT_EXIST) {
		strcpy(response, "QUESTION_DOESNT_EXIST\0");
		return response;
	} else if (error_code == QUESTION_ALREADY_EXISTS) {
		strcpy(response, "QUESTION_ALREADY_EXISTS\0");
		return response;
	}

	return NULL;
}

char* udp_manager(char *request) {
	int i, error_code;
	char protocol[PROTOCOL_SIZE + 1];
	char user_id[USER_ID_SIZE + 1];
	char topic[TOPIC_SIZE + 1];

	char **topics_list;
	char **questions_list;

	for (i = 0; i < PROTOCOL_SIZE && request[i] != '\0' && request[i] != '\n';  i++) {
		protocol[i] = request[i];
	}

	protocol[i] = '\0';

	if (!strcmp(protocol, REG)) {

		error_code = parse_input_REG(request, user_id);
		if (error_code) {
			return parse_output_ERROR(error_code);
		}

		error_code = register_user(user_id);
		if (error_code) {
			return parse_output_ERROR(error_code);
		}

		return parse_output_RGR();

	} else if (!strcmp(protocol, LTP)) {

		error_code = parse_input_LTP(request);
		if (error_code) {
			return parse_output_ERROR(error_code);
		}

		error_code = topic_list(&topics_list);
		if (error_code) {
			return parse_output_ERROR(error_code);
		}

		return parse_output_LTR(topics_list);

	} else if (!strcmp(protocol, PTP)) {

		error_code = parse_input_PTP(request, user_id, topic);
		if (error_code) {
			return parse_output_ERROR(error_code);
		}

		error_code = propose_topic(user_id, topic);
		if (error_code) {
			return parse_output_ERROR(error_code);
		}

		return parse_output_PTR();

	} else if (!strcmp(protocol, LQU)) {

		error_code = parse_input_LQU(request, topic);
		if (error_code) {
			return parse_output_ERROR(error_code);
		}

		error_code = list_questions(topic, &questions_list);
		if (error_code) {
			return parse_output_ERROR(error_code);
		}

		return parse_output_LQR(topic, questions_list);
	}
	// no protocol
	// parse_output_ERROR();
	return NULL;
}