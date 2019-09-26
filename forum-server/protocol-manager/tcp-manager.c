

#include "tcp-manager.h"
#include "protocol.h"
#include "../entities/question.h"
#include "../exceptions.h"

#include <stdbool.h>

char *tcp_manager(char *request) {
	int  i, error_code;
	char protocol[PROTOCOL_SIZE + 1];
	char topic[TOPIC_SIZE + 1];
	char question_title[QUESTION_SIZE + 1];
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
		// get_question(topic, question_title, &question);
		return parse_output_QGR(question); -> free_question
	} else if (!strcmp(protocol, QUS)) {
		error_code = parse_input_QUS(request, topic, &question);
		if (error_code) {
			printf("ERROR on QUS\n");
			return parse_output_ERROR(error);
		}
		// question_submit(topic, question) -> free_question()
		return parse_output_QUR()
	} else if (!strcmp(protocol, ANS)) {
		error_code = parse_input_ANS(request, topic, question_title, &answer);
		if (error_code) {
			printf("ERROR on ANS\n");
			return parse_output_ERROR(error_code);
		}
		// answer_submit(topic, question_title, answer) -> free_answer()
		return parse_output_ANR()
	}

	return parse_output_ERROR(BAD_INPUT);
}

/*
 * QUESTION GET
*/
static int parse_input_GQU(char *request, char *topic, char *question_title) {
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

	for (j = 0; j < QUESTION_TITLE_SIZE && request[i] != '\0' && request[i] != '\n'; i++, j++) {
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
	// parse the question to the protocol string
	return NULL;
}

/*
 * QUESTION SUBMIT
*/
static int parse_input_QUS(char *request, char *topic, question_t **question) {
	int  i, j;
	int  question_size = 0;
	char question_user_id[MAX_TITLE+1];
	char question_title[QUESTION_TITLE_SIZE+1];
	char question_data[QUESTION_SIZE+1];
	bool has_question_img;
	int  question_img_size = 0;
	char question_img_ext[IMAGE_EXT_SIZE+1];
	char question_img_data[IMAGE_SIZE+1];

	if (request[PROTOCOL_SIZE] != ' ') {
		return BAD_INPUT;
	}

	for (i = PROTOCOL_SIZE + 1, j = 0; j < USER_ID_SIZE && request[i] != '\0' && request[i] != '\n'; i++, j++) {
		if (request[i] < '0' || request[i] > '9') {
			return BAD_INPUT;
		}
		question_user_id[j] = request[i];
	}
	question_user_id[j] = '\0';

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

	if (request[i++] != ' ') {
		return BAD_INPUT;
	}

	for (j = 0; j < QUESTION_TITLE_SIZE && request[i] != '\0' && request[i] != '\n'; i++, j++) {
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
		question_size += request[i] - '0';
		question_size *= 10;
	}
	if (question_size == 0) {
		return BAD_INPUT;
	}
	question_size /= 10;

	if (request[i++] != ' ') {
		return BAD_INPUT;
	}

	for (j = 0; j < question_size && j < QUESTION_SIZE && request[i] != '\0'; i++, j++) {
		question_data[j] = request[i];
	}
	question_data[j] = '\0';

	if (request[i++] != ' ' || j != question_size) {
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
			question_img_ext[j] = request[i];
		}
		question_img_ext[j] = '\0';

		if (request[i++] != 0) {
			return BAD_INPUT;
		}

		for (j = 0; request[i] != '\0' && request[i] != '\n' && request[i] != ' '; i++, j++) {
			if (request[i] < '0' || request[i] > '9') {
				return BAD_INPUT;
			}
			question_img_size += request[i] - '0';
			question_img_size *= 10;
		}
		if (question_img_size == 0) {
			return BAD_INPUT;
		}
		question_img_size /= 10;

		if (request[i++] != ' ') {
			return BAD_INPUT;
		}

		for (j = 0; j < question_img_size && j < IMAGE_SIZE && request[i] != '\0'; i++, j++) {
			question_img_data[j] = request[i];
		}
		question_img_data[j] = '\0';

		if (request[i] != ' ' && request[i+1] != '\0') {
			return BAD_INPUT;
		}

		*question = new_question(question_title, question_size, question_data, question_img_size, question_img_data, NULL);
		return SUCCESS;

	} else if (request[i++] == '0') {
		if (request[i] != ' ' && request[i+1] != '\0') {
			return BAD_INPUT;
		}

		*question = new_question(question_title, question_size, question_data, 0, NULL, NULL);
		return SUCCESS;
	}

	return BAD_INPUT;
}

static char *parse_output_QUR() {
	// make the success protocol string
	return NULL;
}

/*
 * ANSWER SUBMIT
*/
static int parse_input_ANS(char *request, char *topic, char *question_title, answer_t **answer) {
	int  i, j;
	char answer_user_id[MAX_TITLE+1];
	char answer_title[ANSWER_TITLE_SIZE+1];
	int  answer_size = 0;
	char answer_data[ANSWER_SIZE+1];
	bool has_answer_img;
	int  answer_img_size = 0;
	char answer_img_ext[IMAGE_EXT_SIZE+1];
	char answer_img_data[IMAGE_SIZE+1];

	if (request[PROTOCOL_SIZE] != ' ') {
		return BAD_INPUT;
	}

	for (i = PROTOCOL_SIZE + 1, j = 0; j < USER_ID_SIZE && request[i] != '\0' && request[i] != '\n'; i++, j++) {
		if (request[i] < '0' || request[i] > '9') {
			return BAD_INPUT;
		}
		question_user_id[j] = request[i];
	}
	question_user_id[j] = '\0';

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

	if (request[i++] != ' ') {
		return BAD_INPUT;
	}

	for (j = 0; j < QUESTION_TITLE_SIZE && request[i] != '\0' && request[i] != '\n'; i++, j++) {
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

	for (j = 0; j < answer_size && j < QUESTION_SIZE && request[i] != '\0'; i++, j++) {
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

		for (j = 0; j < answer_img_size && j < IMAGE_SIZE && request[i] != '\0'; i++, j++) {
			answer_img_data[j] = request[i];
		}
		answer_img_data[j] = '\0';

		if (request[i] != ' ' && request[i+1] != '\0') {
			return BAD_INPUT;
		}

		*answer = new_answer(answer_title, answer_size, answer_data, answer_img_size, answer_img_data, NULL);
		return SUCCESS;

	} else if (request[i++] == '0') {
		if (request[i] != ' ' && request[i+1] != '\0') {
			return BAD_INPUT;
		}

		*answer = new_answer(answer_title, answer_size, answer_data, 0, NULL, NULL);
		return SUCCESS;
	}

	return BAD_INPUT;
}

static char *parse_output_ANR() {
	// make the success protocol string
	return NULL;
}

/*
 * ERROR PROTOCOL
*/
static char *parse_output_ERROR(int error_code) {
	// check wich error and turn it into the respective protocol error
	return NULL;
}
