/* _______________________________________________________________________________
 * »»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
 * IST - RC 2019/2020
 *
 * list-questions-io.c
 *
 * -------------------------------------------------------------------------------
 *
 * Made by: Bruno Meira (89421), Nikoletta Matsur (89513) and Vasco Faria (89559)
 *
 * «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««
 */

#include "list-questions-io.h"

#include "../protocol.h"
#include "../exceptions.h"
#include "../constants.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int parse_input_LQU(char* request, char* topic) {
	int i, j;

	if (request[PROTOCOL_SIZE] != ' ') {
		return BAD_INPUT;
	}

	for (i = PROTOCOL_SIZE + 1, j = 0; j < MAX_TOPIC_TITLE && request[i] != '\0' && request[i] != '\n'; i++, j++) {
		if (request[i] == ' ') {
			return BAD_INPUT;
		}
		topic[j] = request[i];
	}
	topic[j] = '\0';

	return SUCCESS;
}

char *parse_output_LQR(char* topic, char **questions_list) {
	int  num = 0;
	char num_str[3] = "\0";
	
	char questions[MAX_TOPIC_LIST_RESPONSE] = "\0";
	char *response = (char*) malloc(sizeof(char)*(MAX_TOPIC_LIST_RESPONSE + 6));
	response[0] = '\0';
	
	for(num = 0; num < MAX_TOPIC_LIST_RESPONSE && questions_list[num] != NULL; num++){
		strcat(questions, " ");
		strcat(questions, questions_list[num]);
	}

	sprintf(num_str, "%d", num);

	strcpy(response, LQR);
	strcat(response, " ");
	strcat(response, num_str);

	if (num != 0) {
		strcat(response, questions);
	}

	strcat(response, "\n\0");

	return response;
}

char *parse_output_ERROR_LQR(int error_code) {
	
	char* response = (char*) malloc(sizeof(char)*(MAX_STATUS_RESPONSE+1));

	if (error_code == BAD_INPUT || error_code == FAILURE) {
		strcpy(response, "ERR\n\0");

	} else if (error_code == TOPIC_DOESNT_EXIST) {
		strcpy(response, "ERR\n\0");

	} else {
		strcpy(response, "ERR\n\0");
	}
	return response;
}
