/* _______________________________________________________________________________
 * »»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
 * IST - RC 2019/2020
 *
 * udp-manager.c
 *
 * -------------------------------------------------------------------------------
 *
 * Made by: Bruno Meira (89421), Nikoletta Matsur (89513) and Vasco Faria (89559)
 *
 * «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««
 */

#include "udp-manager.h"

#include "../protocol.h"
#include "../exceptions.h"
#include "../constants.h"
#include "../usecases/usecases.h"

#include "register-io.h"
#include "list-topics-io.h"
#include "propose-topic-io.h"
#include "list-questions-io.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * ERROR PROTOCOL
*/
static char *parse_output_ERROR(int error_code) {
	
	char* response = (char*) malloc(sizeof(char)*(MAX_STATUS_RESPONSE+1));

	strcpy(response, "ERR\n\0");

	return response;
}

char *udp_manager(char *request) {
	int i, error_code;
	char protocol[PROTOCOL_SIZE + 1];
	char user_id[USER_ID_SIZE + 1];
	char topic[MAX_TOPIC_TITLE + 1];

	char **topics_list;
	char **questions_list;

	for (i = 0; i < PROTOCOL_SIZE && request[i] != '\0' && request[i] != '\n';  i++) {
		protocol[i] = request[i];
	}

	protocol[i] = '\0';

	if (!strcmp(protocol, REG)) {

		error_code = parse_input_REG(request, user_id);
		if (error_code) {
			return parse_output_ERROR_RGR(error_code);
		}

		error_code = register_user(user_id);
		if (error_code) {
			return parse_output_ERROR_RGR(error_code);
		}

		return parse_output_RGR();

	} else if (!strcmp(protocol, LTP)) {

		error_code = parse_input_LTP(request);
		if (error_code) {
			return parse_output_ERROR_LTR(error_code);
		}

		error_code = topic_list(&topics_list);
		if (error_code) {
			return parse_output_ERROR_LTR(error_code);
		}

		return parse_output_LTR(topics_list);

	} else if (!strcmp(protocol, PTP)) {

		error_code = parse_input_PTP(request, user_id, topic);
		if (error_code) {
			return parse_output_ERROR_PTR(error_code);
		}

		error_code = propose_topic(user_id, topic);
		if (error_code) {
			return parse_output_ERROR_PTR(error_code);
		}

		return parse_output_PTR();

	} else if (!strcmp(protocol, LQU)) {

		error_code = parse_input_LQU(request, topic);
		if (error_code) {
			return parse_output_ERROR_LQR(error_code);
		}

		error_code = list_questions(topic, &questions_list);
		if (error_code) {
			return parse_output_ERROR_LQR(error_code);
		}

		return parse_output_LQR(topic, questions_list);
	}
	return parse_output_ERROR(FAILURE);
}
