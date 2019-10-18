/* _______________________________________________________________________________
 * »»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
 * IST - RC 2019/2020
 *
 * list-topics-io.c
 *
 * -------------------------------------------------------------------------------
 *
 * Made by: Bruno Meira (89421), Nikoletta Matsur (89513) and Vasco Faria (89559)
 *
 * «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««
 */

#include "list-topics-io.h"

#include "../protocol.h"
#include "../exceptions.h"
#include "../constants.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int parse_input_LTP(char* request) {

	if (request[PROTOCOL_SIZE] != '\n' || request[PROTOCOL_SIZE+1] != '\0') {
		return BAD_INPUT;
	}

	return SUCCESS;
}

char *parse_output_LTR(char **topics_list) {
	int  num;
	char num_str[3];
	char topics[MAX_TOPIC_LIST_RESPONSE] = "\0";
	char *response = (char*) malloc(sizeof(char)*(MAX_TOPIC_LIST_RESPONSE + PROTOCOL_SIZE + 3));
	response[0] = '\0';

	for(num = 0; num < MAX_TOPIC_LIST_RESPONSE && topics_list[num] != NULL; num++) {
	    strcat(topics, " ");
	    strcat(topics, topics_list[num]);
	}

	sprintf(num_str, "%d", num);

	strcat(response, LTR);
	strcat(response, " ");
	strcat(response, num_str);

	if (num != 0) strcat(response, topics);
	
	strcat(response, "\n\0");
	
	return response;
}

char *parse_output_ERROR_LTR(int error_code) {
	
	char* response = (char*) malloc(sizeof(char)*(MAX_STATUS_RESPONSE+1));

	if (error_code == BAD_INPUT || error_code == FAILURE) {
		strcpy(response, "ERR\n\0");

	} else {
		strcpy(response, "ERR\n\0");
	}
	return response;
}
