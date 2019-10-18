/* _______________________________________________________________________________
 * »»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
 * IST - RC 2019/2020
 *
 * propose-topic-io.c
 *
 * -------------------------------------------------------------------------------
 *
 * Made by: Bruno Meira (89421), Nikoletta Matsur (89513) and Vasco Faria (89559)
 *
 * «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««
 */

#include "propose-topic-io.h"

#include "../protocol.h"
#include "../exceptions.h"
#include "../constants.h"

#include <stdlib.h>
#include <string.h>


int parse_input_PTP(char* request, char* user_id, char* topic) {
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

	for (j = 0; j < MAX_TOPIC_TITLE && request[i] != '\0' && request[i] != '\n'; i++, j++) {
		if (request[i] == ' ') {
			return BAD_INPUT;
		}
		topic[j] = request[i];
	}
	topic[j] = '\0';

	return SUCCESS;
}

char *parse_output_PTR() {

	char *response = (char*) malloc(sizeof(char)*(MAX_STATUS_RESPONSE+1));
	response[0] = '\0';
	strcat(response, PTR);
	strcat(response, OK);
	strcat(response, "\n\0");
	return response;
}

char *parse_output_ERROR_PTR(int error_code) {
	
	char* response = (char*) malloc(sizeof(char)*(MAX_STATUS_RESPONSE+1));
	if (error_code == BAD_INPUT || error_code == FAILURE) {
		strcpy(response, "PTR NOK\n\0");

	} else if (error_code == TOPIC_ALREADY_EXISTS) {
		strcpy(response, "PTR DUP\n\0");

	} else if (error_code == MAX_TOPICS_REACHED) {
		strcpy(response, "PTR FUL\n\0");

	} else {
		strcpy(response, ERR);
		strcpy(response, "PTR NOK\n\0");
	}
	return response;
}
