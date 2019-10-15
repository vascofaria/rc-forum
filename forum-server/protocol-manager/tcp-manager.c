/* _______________________________________________________________________________
 * »»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
 * IST - RC 2019/2020
 *
 * tcp-manager.c
 *
 * -------------------------------------------------------------------------------
 *
 * Made by: Bruno Meira (89421), Nikoletta Matsur (89513) and Vasco Faria (89559)
 *
 * «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««
 */

#include "tcp-manager.h"
#include "protocol.h"

#include "../entities/question.h"
#include "../entities/answer.h"

#include "../usecases/usecases.h"
#include "../exceptions.h"

#include "question-get-io.h"
#include "question-submit-io.h"
#include "answer-submit-io.h"

#include <stdio.h>
#include <string.h>


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
