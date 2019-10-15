/* _______________________________________________________________________________
 * »»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
 * IST - RC 2019/2020
 *
 * question-submit-io.c
 *
 * -------------------------------------------------------------------------------
 *
 * Made by: Bruno Meira (89421), Nikoletta Matsur (89513) and Vasco Faria (89559)
 *
 * «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««
 */

#include "question-submit-io.h"

#include "protocol.h"
#include "../exceptions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_input_QUS(char *request, char *topic, question_t **question) {
	int  i, j;
	int  question_size = 0;
	char question_user_id[USER_ID_SIZE+1];
	char question_title[MAX_QUESTION_TITLE+1];
	char question_data[MAX_TXT_SIZE+1];
	int  question_img_size = 0;
	char question_img_ext[IMAGE_EXT_SIZE+1];
	char question_img_data[MAX_IMG_SIZE+1];

	if (request[PROTOCOL_SIZE] != ' ') {
		return BAD_INPUT;
	}

	printf("a\n");

	for (i = PROTOCOL_SIZE + 1, j = 0; j < USER_ID_SIZE && request[i] != '\0' && request[i] != '\n'; i++, j++) {
		if (request[i] < '0' || request[i] > '9') {
			return BAD_INPUT;
		}
		question_user_id[j] = request[i];
	}
	question_user_id[j] = '\0';

	printf("b\n");

	if (request[i++] != ' ') {
		return BAD_INPUT;
	}

	printf("c\n");

	for (j = 0; j < MAX_TOPIC_TITLE && request[i] != '\0' && request[i] != '\n' && request[i] != ' '; i++, j++) {
		topic[j] = request[i];
	}
	topic[j] = '\0';

	printf("d\n");

	if (request[i++] != ' ') {
		return BAD_INPUT;
	}

	printf("d\n");

	for (j = 0; j < MAX_QUESTION_TITLE && request[i] != '\0' && request[i] != '\n' && request[i] != ' '; i++, j++) {
		question_title[j] = request[i];
	}
	question_title[j] = '\0';

	printf("e\n");

	if (request[i++] != ' ') {
		return BAD_INPUT;
	}

	printf("f\n");

	for (j = 0; request[i] != '\0' && request[i] != '\n' && request[i] != ' '; i++, j++) {
		if (request[i] < '0' || request[i] > '9') {
			return BAD_INPUT;
		}
		question_size += request[i] - '0';
		question_size *= 10;
	}
	printf("g\n");
	if (question_size == 0) {
		return BAD_INPUT;
	}
	question_size /= 10;
	printf("h\n");

	if (request[i++] != ' ') {
		return BAD_INPUT;
	}
	printf("i\n");
	printf("%s %s %s %d\n", question_user_id, topic, question_title, question_size);

	for (j = 0; j < question_size && j < MAX_TXT_SIZE; i++, j++) {
		question_data[j] = request[i];
	}
	question_data[j] = '\0';

	if (request[i++] != ' ' || j != question_size) {
		return BAD_INPUT;
	}
	printf("j\n");

	if (request[i] == '1') {
		i++;

		if (request[i++] != ' ') {
			return BAD_INPUT;
		}

		for (j = 0; j < IMAGE_EXT_SIZE && request[i] != '\0' && request[i] != '\n' && request[i] != ' '; i++, j++) {
			question_img_ext[j] = request[i];
		}
		question_img_ext[j] = '\0';
		printf("k\n");

		if (request[i++] != ' ') {
			return BAD_INPUT;
		}

		for (j = 0; request[i] != '\0' && request[i] != '\n' && request[i] != ' '; i++, j++) {
			if (request[i] < '0' || request[i] > '9') {
				return BAD_INPUT;
			}
			question_img_size += request[i] - '0';
			question_img_size *= 10;
		}
		printf("l\n");
		if (question_img_size == 0) {
			return BAD_INPUT;
		}
		question_img_size /= 10;

		if (request[i++] != ' ') {
			return BAD_INPUT;
		}

		for (j = 0; j < question_img_size && j < MAX_IMG_SIZE; i++, j++) {
			question_img_data[j] = request[i];
		}
		printf("m\n");
		question_img_data[j] = '\0';

		if (request[i] != ' ' && request[i+1] != '\0') {
			return BAD_INPUT;
		}

		*question = new_question(question_title, question_user_id, question_size, question_data, question_img_size, question_img_ext, question_img_data, NULL);
		return SUCCESS;

	} else if (request[i++] == '0') {
		if (request[i] != ' ' && request[i+1] != '\0') {
			return BAD_INPUT;
		}
		printf("%s - %s - %d\n", question_title, question_user_id, question_size);
		*question = new_question(question_title, question_user_id, question_size, question_data, 0, NULL, NULL, NULL);
		return SUCCESS;
	}
	printf("n\n");

	return BAD_INPUT;
}


char *parse_output_QUR() {
	char *response = (char*) malloc(sizeof(char)*(MAX_STATUS_RESPONSE+1));
	response[0] = '\0';
	strcat(response, OK);
	strcat(response, "\n\0");
	return response;
}