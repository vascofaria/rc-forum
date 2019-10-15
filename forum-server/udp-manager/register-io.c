/* _______________________________________________________________________________
 * »»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
 * IST - RC 2019/2020
 *
 * register-io.c
 *
 * -------------------------------------------------------------------------------
 *
 * Made by: Bruno Meira (89421), Nikoletta Matsur (89513) and Vasco Faria (89559)
 *
 * «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««
 */

#include "register-io.h"

#include "../protocol.h"
#include "../exceptions.h"
#include "../constants.h"

#include <stdlib.h>
#include <string.h>

int parse_input_REG(char* request, char* user_id) {
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


char *parse_output_RGR() {
	char *response = (char*) malloc(sizeof(char) * (MAX_STATUS_RESPONSE+1));
	strcpy(response, RGR);
	strcat(response, OK);
	strcat(response, "\n\0");
	return response;
}
