/* _______________________________________________________________________________
 * »»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
 * IST - RC 2019/2020
 *
 * answer-submit-io.c
 *
 * -------------------------------------------------------------------------------
 *
 * Made by: Bruno Meira (89421), Nikoletta Matsur (89513) and Vasco Faria (89559)
 *
 * «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««
 */

#include "answer-submit-io.h"

#include "protocol.h"
#include "../constants.h"
#include "../exceptions.h"

#include <stdlib.h>
#include <string.h>

int parse_input_ANS(char *request, char *topic, char *question_title, answer_t **answer) {
	int  i, j;
	char answer_user_id[USER_ID_SIZE+1];
	char answer_title[MAX_ANSWER_TITLE+1];
	int  answer_size = 0;
	char answer_data[MAX_TXT_SIZE+1];
	int  answer_img_size = 0;
	char answer_img_ext[IMAGE_EXT_SIZE+1];
	char answer_img_data[MAX_IMG_SIZE+1];

	if (request[PROTOCOL_SIZE] != ' ') {
		return BAD_INPUT;
	}

	for (i = PROTOCOL_SIZE + 1, j = 0; j < USER_ID_SIZE && request[i] != '\0' && request[i] != '\n'; i++, j++) {
		if (request[i] < '0' || request[i] > '9') {
			return BAD_INPUT;
		}
		answer_user_id[j] = request[i];
	}
	answer_user_id[j] = '\0';

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

	if (request[i++] != ' ') {
		return BAD_INPUT;
	}

	for (j = 0; j < MAX_QUESTION_TITLE && request[i] != '\0' && request[i] != '\n'; i++, j++) {
		if (request[i] == ' ') {
			return BAD_INPUT;
		}
		question_title[i] = request[i];
	}
	question_title[i] = '\0';

	if (request[i++] != ' ') {
		return BAD_INPUT;
	}

	for (j = 0; request[i] != '\0' && request[i] != '\n' && request[i] != ' '; i++, j++) {
		if (request[i] < '0' || request[i] > '9') {
			return BAD_INPUT;
		}
		answer_size += request[i] - '0';
		answer_size *= 10;
	}
	if (answer_size == 0) {
		return BAD_INPUT;
	}
	answer_size /= 10;

	if (request[i++] != ' ') {
		return BAD_INPUT;
	}

	for (j = 0; j < answer_size && j < MAX_TXT_SIZE && request[i] != '\0'; i++, j++) {
		answer_data[j] = request[i];
	}
	answer_data[j] = '\0';

	if (request[i++] != ' ' || j != answer_size) {
		return BAD_INPUT;
	}

	if (request[i++] == '1') {
		if (request[i++] != ' ') {
			return BAD_INPUT;
		}

		for (j = 0; j < IMAGE_EXT_SIZE && request[i] != '\0' && request[i] != '\n'; i++, j++) {
			if (request[i] == ' ') {
				return BAD_INPUT;
			}
			answer_img_ext[j] = request[i];
		}
		answer_img_ext[j] = '\0';

		if (request[i++] != 0) {
			return BAD_INPUT;
		}

		for (j = 0; request[i] != '\0' && request[i] != '\n' && request[i] != ' '; i++, j++) {
			if (request[i] < '0' || request[i] > '9') {
				return BAD_INPUT;
			}
			answer_img_size += request[i] - '0';
			answer_img_size *= 10;
		}
		if (answer_img_size == 0) {
			return BAD_INPUT;
		}
		answer_img_size /= 10;

		if (request[i++] != ' ') {
			return BAD_INPUT;
		}

		for (j = 0; j < answer_img_size && j < MAX_IMG_SIZE && request[i] != '\0'; i++, j++) {
			answer_img_data[j] = request[i];
		}
		answer_img_data[j] = '\0';

		if (request[i] != ' ' && request[i+1] != '\0') {
			return BAD_INPUT;
		}

		*answer = new_answer(answer_title, answer_user_id, answer_size, answer_data, answer_img_size, answer_img_ext, answer_img_data);
		return SUCCESS;

	} else if (request[i++] == '0') {
		if (request[i] != ' ' && request[i+1] != '\0') {
			return BAD_INPUT;
		}

		*answer = new_answer(answer_title, answer_user_id, answer_size, answer_data, 0, NULL, NULL);
		return SUCCESS;
	}

	return BAD_INPUT;
}

char *parse_output_ANR() {
	char *response = (char*) malloc(sizeof(char)*(MAX_STATUS_RESPONSE+1));
	response[0] = '\0';
	strcat(response, OK);
	strcat(response, "\n\0");
	return response;
}