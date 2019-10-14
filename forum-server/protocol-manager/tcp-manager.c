

#include "tcp-manager.h"
#include "protocol.h"
#include "../entities/question.h"
#include "../usecases/usecases.h"
#include "../exceptions.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
/*
 * QUESTION GET
*/
static int parse_input_GQU(char *request, char *topic, char *question_title) {
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

static char *parse_output_QGR(question_t *question) {
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

/*
 * QUESTION SUBMIT
*/
static int parse_input_QUS(char *request, char *topic, question_t **question) {
	int  i, j;
	int  question_size = 0;
	char question_user_id[USER_ID_SIZE+1];
	char question_title[MAX_QUESTION_TITLE+1];
	char question_data[MAX_TXT_SIZE+1];
	bool has_question_img;
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

static char *parse_output_QUR() {
	char *response = (char*) malloc(sizeof(char)*(MAX_STATUS_RESPONSE+1));
	response[0] = '\0';
	strcat(response, OK);
	strcat(response, "\n\0");
	return response;
}

/*
 * ANSWER SUBMIT
*/
static int parse_input_ANS(char *request, char *topic, char *question_title, answer_t **answer) {
	int  i, j;
	char answer_user_id[USER_ID_SIZE+1];
	char answer_title[MAX_ANSWER_TITLE+1];
	int  answer_size = 0;
	char answer_data[MAX_TXT_SIZE+1];
	bool has_answer_img;
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

static char *parse_output_ANR() {
	char *response = (char*) malloc(sizeof(char)*(MAX_STATUS_RESPONSE+1));
	response[0] = '\0';
	strcat(response, OK);
	strcat(response, "\n\0");
	return response;
}

/*
 * ERROR PROTOCOL
*/
static char *parse_output_ERROR(int error_code) {
	// check wich error and turn it into the respective protocol error
	if (error_code == BAD_INPUT) {
		return "BAD_INPUT\0";
	} else if (error_code == TOPIC_DOESNT_EXIST) {
		return "TOPIC_DOESNT_EXIST\0";
	} else if (error_code == TOPIC_ALREADY_EXISTS) {
		return "TOPIC_ALREADY_EXISTS\0";
	} else if (error_code == QUESTION_DOESNT_EXIST) {
		return "QUESTION_DOESNT_EXIST\0";
	} else if (error_code == QUESTION_ALREADY_EXISTS) {
		return "QUESTION_ALREADY_EXISTS\0";
	}

	return NULL;
}


char *tcp_manager(char *request) {
	int  i, error_code;
	char protocol[PROTOCOL_SIZE + 1];
	char topic[MAX_TOPIC_TITLE + 1];
	char question_title[MAX_QUESTION_TITLE + 1];
	question_t *question;
	answer_t *answer;


	for (i = 0; i < PROTOCOL_SIZE && request[i] != '\0' && request[i] != '\n';  i++) {
		protocol[i] = request[i];
	}
	protocol[i] = '\0';

	if (!strcmp(protocol, GQU)) {

		error_code = parse_input_GQU(request, topic, question_title);
		if (error_code) {
			printf("ERROR on GQU\n");
			return parse_output_ERROR(error_code);
		}
		error_code = search_question(topic, question_title, &question);
		if (error_code) {
			printf("ERROR on search_question\n");
			return parse_output_ERROR(error_code);
		}
		return parse_output_QGR(question); // -> free_question

	} else if (!strcmp(protocol, QUS)) {

		error_code = parse_input_QUS(request, topic, &question);
		if (error_code) {
			printf("ERROR on QUS\n");
			return parse_output_ERROR(error_code);
		}
		error_code = question_submit(topic, question);
		if (error_code) {
			printf("ERROR on question_submit\n");
			return parse_output_ERROR(error_code);
		}
		free_question(question);
		return parse_output_QUR();

	} else if (!strcmp(protocol, ANS)) {

		error_code = parse_input_ANS(request, topic, question_title, &answer);
		if (error_code) {
			printf("ERROR on ANS\n");
			return parse_output_ERROR(error_code);
		}
		error_code = answer_submit(topic, question_title, answer);
		if (error_code) {
			printf("ERROR on answer_submit\n");
			return parse_output_ERROR(error_code);
		}
		free_answer(answer);
		return parse_output_ANR();
	}

	return parse_output_ERROR(BAD_INPUT);
}
