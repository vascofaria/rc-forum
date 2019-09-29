#include "udp-manager.h"
#include "protocol.h"
#include "../exceptions.h"
#include "../usecases/usecases.h"
#include <stdio.h>
#include <string.h>

#define PROTOCOL_SIZE 3
#define USER_ID_SIZE  5
#define TOPIC_SIZE    10

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

static int parse_input_LTP(char* request) {

	if (request[PROTOCOL_SIZE] != '\n' || request[PROTOCOL_SIZE+1] != '\0') {
		return BAD_INPUT;
	}

	return SUCCESS;
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

char* udp_manager(char *request) {
	int i, error_code;
	char protocol[PROTOCOL_SIZE + 1];
	char user_id[USER_ID_SIZE + 1];
	char topic[TOPIC_SIZE + 1];

	for (i = 0; i < PROTOCOL_SIZE && request[i] != '\0' && request[i] != '\n';  i++) {
		protocol[i] = request[i];
	}

	protocol[i] = '\0';

	if (!strcmp(protocol, REG)) {
		error_code = parse_input_REG(request, user_id);
		// if (error_code) {
		// 	printf("ERROR on REG\n");
		// }
		// printf("OK\n");
		// usecase
		return parse_output_RGR(error_code);
	} else if (!strcmp(protocol, LTP)) {
		error_code = parse_input_LTP(request);
		// if (error_code) {
		// 	printf("ERROR on LTP\n");
		// }
		// printf("OK\n");
		return parse_output_LTR(error_code);
	} else if (!strcmp(protocol, PTP)) {
		error_code = parse_input_PTP(request, user_id, topic);
		if (error_code) {
			printf("ERROR on PTP\n");
		}
		printf("OK\n");
	} else if (!strcmp(protocol, LQU)) {
		error_code = parse_input_LQU(request, topic);
		if (error_code) {
			printf("ERROR on LQU\n");
		}
		printf("OK\n");
	} else {
		printf("ERROR on protocol\n");
	}
	return NULL;

	/* TO DO: acabar o resto para os restantes protocolos*/
}