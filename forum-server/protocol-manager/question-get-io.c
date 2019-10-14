/* _______________________________________________________________________________
 * »»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
 * IST - RC 2019/2020
 *
 * question-get-io.c
 *
 * -------------------------------------------------------------------------------
 *
 * Made by: Bruno Meira (89421), Nikoletta Matsur (89513) and Vasco Faria (89559)
 *
 * «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««
 */

#include "question-get-io.h"

#include "protocol.h"
#include "../entities/answer.h"
#include "../exceptions.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int parse_input_GQU(char *request, char *topic, char *question_title) {
	int i, j;

	if (request[PROTOCOL_SIZE] != ' ') {
		return BAD_INPUT;
	}

	for (i = PROTOCOL_SIZE + 1, j = 0; j < MAX_TOPIC_TITLE && request[i] != '\0' && request[i] != '\n' && request[i] != ' '; i++, j++) {
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
		question_title[j] = request[i];
	}
	question_title[j] = '\0';

	if (request[i] != '\n' || request[i+1] != '\0') {
		return BAD_INPUT;
	}

	return SUCCESS;
}


char *parse_output_QGR(question_t *question) {
	int answers_number = 0, i, j;
	char answers_number_str[3], data_size_str[MAX_TXT_SIZE+1], image_size_str[MAX_IMG_SIZE+1];
	char *response = (char*) malloc(sizeof(char)*(MAX_STATUS_RESPONSE+1));
	response[0] = '\0';
	strcat(response, QGR);

	strcat(response, question->user_id);
	strcat(response, " \0");

	sprintf(data_size_str, "%d", question->data_size);
	strcat(response, data_size_str);
	strcat(response, " \0");

	strcat(response, question->data);
	strcat(response, " \0");

	if (question->image_size) {
		strcat(response, "1 \0");

		strcat(response, question->image_ext);
		strcat(response, " \0");

		sprintf(image_size_str, "%d", question->image_size);
		strcat(response, image_size_str);
		strcat(response, " \0");

		for (i = strlen(response), j = 0; i < MAX_STATUS_RESPONSE && j < question->image_size; i++, j++) {
			response[i] = question->image[j];
		}
		response[i] = '\0';

	} else {
		strcat(response, "0 \0");
	}

	for (i = 0; i < MAX_ANSWERS && question->answers[i] != NULL; i++) {
		answers_number++;
	}
	if (answers_number < 10) {
		answers_number_str[0] = '0' + answers_number;
		answers_number_str[1] = '\0';
	} else {
		strcpy(answers_number_str, "10\0");
	}

	strcat(response, answers_number_str);

	for (i = 0; i < answers_number && question->answers[i] != NULL; i++) {
		strcat(response, " \0");

		strcat(response, question->answers[i]->title);
		strcat(response, " \0");

		strcat(response, question->answers[i]->user_id);
		strcat(response, " \0");

		sprintf(data_size_str, "%d", question->answers[i]->data_size);
		strcat(response, data_size_str);
		strcat(response, " \0");

		strcat(response, question->answers[i]->data);
		strcat(response, " \0");

		if (question->answers[i]->image_size) {
			strcat(response, "1 \0");

			strcat(response, question->answers[i]->image_ext);
			strcat(response, " \0");

			sprintf(image_size_str, "%d", question->answers[i]->image_size);
			strcat(response, image_size_str);
			strcat(response, " \0");

			strcat(response, question->answers[i]->image);
		} else {
			strcat(response, "0\0");
		}
	}

	strcat(response, "\n\0");
	return response;
}
