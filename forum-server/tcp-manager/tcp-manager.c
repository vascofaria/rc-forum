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

#include "../protocol.h"

#include "../entities/question.h"
#include "../entities/answer.h"

#include "../usecases/usecases.h"
#include "../exceptions.h"

#include "question-get-io.h"
#include "question-submit-io.h"
#include "answer-submit-io.h"

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

/*
 * WRITE TO TCP SOCKET
*/
int write_to_tcp_socket(int socket_tcp, char *buffer, char final_char) {
	int len, n;
	char *buffer_ptr = buffer;

	len = strlen(buffer);

	while(len > 0) {
		n = write(socket_tcp, buffer_ptr, len);
		if (n == -1) {
			fprintf(stderr, "write to socket failed: %s\n", strerror(errno));
			return FAILURE;
		}
		len -= n;
		buffer_ptr += (sizeof(char) * n);
	}

	if (final_char != '\0') {
		do {
			n = write(socket_tcp, &final_char, 1);
			if (n == -1) {
				fprintf(stderr, "write to socket failed: %s\n", strerror(errno));
				return FAILURE;
			}
		} while(n == 0);
	}

	return SUCCESS;
}

/*
 * READ TO TCP SOCKET
*/
int read_from_tcp_socket(int socket_tcp, char *buffer, int size, char final_char) {
	int len, n;
	char *buffer_ptr = buffer;
	char space[2];

	len = size;

	while (len > 0) {
		n = read(socket_tcp, buffer_ptr, 1);
		if (n == -1) {
			fprintf(stderr, "read from socket failed: %s\n", strerror(errno));
			return FAILURE;
		}
		if (*buffer_ptr == ' ' || *buffer_ptr == '\n' || *buffer_ptr == '\0') {
			if (*buffer_ptr != final_char) {
				return FAILURE;
			}
			*buffer_ptr = '\0';
			return SUCCESS;
		}
		len--;
		buffer_ptr += (sizeof(char));
	}

	if (size == 0) {
		do {
			n = read(socket_tcp, space, 1);
			if (n == -1) {
				fprintf(stderr, "read from socket failed: %s\n", strerror(errno));
				return FAILURE;
			}
		}	while (n == 0);
		if (space[0] == final_char) {
			return SUCCESS;
		}
	} else if (final_char == '\0') {
		return SUCCESS;
	}
	return FAILURE;
}

/*
 * ERROR PROTOCOL
*/
static void parse_output_ERROR(int socket_tcp, int error_code) {
	write_to_tcp_socket(socket_tcp, "ERR\0", '\n');
}

void tcp_manager(int socket_tcp) {
	int  i, error_code;
	char protocol[PROTOCOL_SIZE + 1];
	char topic[MAX_TOPIC_TITLE + 1];
	char question_title[MAX_QUESTION_TITLE + 1];
	question_t *question;
	answer_t *answer;

	error_code = read_from_tcp_socket(socket_tcp, protocol, PROTOCOL_SIZE+1, ' ');
	if (error_code) {
		parse_output_ERROR(socket_tcp, error_code);
		return;
	}

	if (!strcmp(protocol, GQU)) {

		error_code = parse_input_GQU(socket_tcp, topic, question_title);
		if (error_code) {
			printf("ERROR on GQU\n\n");
			parse_output_ERROR_QGR(socket_tcp, error_code);
		} else {
			printf("GQU parse successful\n\n");
		}

		error_code = search_question(topic, question_title, &question);
		if (error_code) {
			printf("ERROR on search_question\n\n");
			parse_output_ERROR_QGR(socket_tcp, error_code);
		} else {
			printf("search_question Usecase successful\n\n");
		}

		error_code = parse_output_QGR(socket_tcp, question); // -> free_question
		if (error_code) {
			printf("ERROR on QGR\n\n");
			parse_output_ERROR_QGR(socket_tcp, error_code);
		} else {
			printf("QGR parse successful\n\n");
		}

	} else if (!strcmp(protocol, QUS)) {

		error_code = parse_input_QUS(socket_tcp, topic, &question);
		if (error_code) {
			printf("ERROR on QUS\n\n");
			parse_output_ERROR_QUR(socket_tcp, error_code);
		} else {
			printf("QUS parse successful\n\n");
		}

		error_code = question_submit(topic, question);
		if (error_code) {
			printf("ERROR on question_submit\n\n");
			parse_output_ERROR_QUR(socket_tcp, error_code);
		} else {
			printf("question_submit Usecase successful\n\n");
		}
		free_question(question);

		error_code = parse_output_QUR(socket_tcp);
		if (error_code) {
			printf("ERROR on QUR\n\n");
			parse_output_ERROR_QUR(socket_tcp, error_code);
		} else {
			printf("QUR parse successful\n\n");
		}

	} else if (!strcmp(protocol, ANS)) {

		error_code = parse_input_ANS(socket_tcp, topic, question_title, &answer);
		if (error_code) {
			printf("ERROR on ANS\n\n");
			parse_output_ERROR_ANR(socket_tcp, error_code);
		} else {
			printf("ANS parse successful\n\n");
		}

		error_code = answer_submit(topic, question_title, answer);
		if (error_code) {
			printf("ERROR on answer_submit\n\n");
			parse_output_ERROR_ANR(socket_tcp, error_code);
		} else {
			printf("answer_submit Usecase successful\n\n");
		}
		free_answer(answer);

		error_code = parse_output_ANR(socket_tcp);
		if (error_code) {
			printf("ERROR on ANR\n\n");
			parse_output_ERROR_ANR(socket_tcp, error_code);
		} else {
			printf("ANR parse successful\n\n");
		}
	}

	parse_output_ERROR(socket_tcp, BAD_INPUT);
}
