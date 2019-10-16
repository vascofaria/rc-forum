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
#include "tcp-manager.h"

#include "../file-manager/file-manager.h"
#include "../protocol.h"
#include "../entities/answer.h"
#include "../exceptions.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int parse_input_GQU(int socket_tcp, char *topic, char *question_title) {
	// TODO: Read the hole line to get the size
	int error_code;

	error_code = read_from_tcp_socket(socket_tcp, topic, MAX_TOPIC_TITLE + 1, ' ');
	if (error_code) {
		return BAD_INPUT;
	}

	error_code = read_from_tcp_socket(socket_tcp, question_title, MAX_QUESTION_TITLE + 1, '\n');
	if (error_code) {
		return BAD_INPUT;
	}

	return SUCCESS;
}


int parse_output_QGR(int socket_tcp, question_t *question) {
	int  answers_number = 0, i, j, error_code;
	char answers_number_str[3], answer_number_str[2], data_size_str[MAX_TXT_SIZE+1], image_size_str[MAX_IMG_SIZE+1];

	error_code = write_to_tcp_socket(socket_tcp, QGR, ' ');
	if (error_code) {
		return FAILURE;
	}

	printf("QGR: %s\n", QGR);
	error_code = write_to_tcp_socket(socket_tcp, question->user_id, ' ');
	if (error_code) {
		return FAILURE;
	}

	printf("User ID: %s\n", question->user_id);

	sprintf(data_size_str, "%d", question->data_size);
	error_code = write_to_tcp_socket(socket_tcp, data_size_str, ' ');
	if (error_code) {
		return FAILURE;
	}

	printf("Data Size: %s\n", data_size_str);

	error_code = write_from_file_to_socket(socket_tcp, question->data_path, question->data_size);
	if (error_code) {
		return FAILURE;
	}
	printf("Data Path: %s\n", question->data_path);

	error_code = write_to_tcp_socket(socket_tcp, "\0", ' ');
	if (error_code) {
		return FAILURE;
	}

	printf("space\n");

	if (question->image_size) {

		error_code = write_to_tcp_socket(socket_tcp, "1\0", ' ');
		if (error_code) {
			return FAILURE;
		}
		printf("number 1\n");

		error_code = write_to_tcp_socket(socket_tcp, question->image_ext, ' ');
		if (error_code) {
			return FAILURE;
		}

		printf("Image Ext: %s\n", question->image_ext);

		sprintf(image_size_str, "%d", question->image_size);
		error_code = write_to_tcp_socket(socket_tcp, image_size_str, ' ');
		if (error_code) {
			return FAILURE;
		}

		printf("Image Size: %s\n", image_size_str);

		error_code = write_from_file_to_socket(socket_tcp, question->image_path, question->image_size);
		if (error_code) {
			return FAILURE;
		}

		printf("Image Path: %s\n", question->image_path);

		error_code = write_to_tcp_socket(socket_tcp, "\0", ' ');
		if (error_code) {
			return FAILURE;
		}

		printf("space\n");

	} else {
		error_code = write_to_tcp_socket(socket_tcp, "0\0", ' ');
		if (error_code) {
			return FAILURE;
		}

		printf("No img : 0\n");
	}

	for (answers_number = 0; answers_number < MAX_ANSWERS && question->answers[answers_number] != NULL; answers_number++);
	if (answers_number < 10) {
		answers_number_str[0] = '0';
		answers_number_str[1] = '0' + answers_number;
		answers_number_str[2] = '\0';
	} else {
		strcpy(answers_number_str, "10\0");
	}

	printf("Answers Number: %s\n", answers_number_str);

	error_code = write_to_tcp_socket(socket_tcp, answers_number_str, '\0');
	if (error_code) {
		return FAILURE;
	}

	// write response

	for (i = 0; i < answers_number && question->answers[i] != NULL; i++) {

		error_code = write_to_tcp_socket(socket_tcp, "\0", ' ');
		if (error_code) {
			return FAILURE;
		}
		printf("space\n");

		error_code = write_to_tcp_socket(socket_tcp, question->answers[i]->number, ' ');
		if (error_code) {
			return FAILURE;
		}

		printf("Actual AnswerNumber: %s\n", question->answers[i]->number);

		printf("space\n");

		error_code = write_to_tcp_socket(socket_tcp, question->answers[i]->user_id, ' ');
		if (error_code) {
			return FAILURE;
		}

		printf("Answer User Id: %s\n", question->answers[i]->user_id);

		sprintf(data_size_str, "%d", question->answers[i]->data_size);
		error_code = write_to_tcp_socket(socket_tcp, data_size_str, ' ');
		if (error_code) {
			return FAILURE;
		}

		printf("Answer size: %s\n", data_size_str);

		error_code = write_from_file_to_socket(socket_tcp, question->answers[i]->data_path, question->answers[i]->data_size);
		if (error_code) {
			return FAILURE;
		}

		printf("Answer Data Path: %s\n", question->answers[i]->data_path);

		error_code = write_to_tcp_socket(socket_tcp, "\0", ' ');
		if (error_code) {
			return FAILURE;
		}

		printf("space\n");

		if (question->answers[i]->image_size) {

			error_code = write_to_tcp_socket(socket_tcp, "1\0", ' ');
			if (error_code) {
				return FAILURE;
			}

			printf("Has image: 1\n");

			error_code = write_to_tcp_socket(socket_tcp, question->answers[i]->image_ext, ' ');
			if (error_code) {
				return FAILURE;
			}

			printf("Answer Image Ext: %s\n", question->answers[i]->image_ext);

			sprintf(image_size_str, "%d", question->answers[i]->image_size);
			error_code = write_to_tcp_socket(socket_tcp, image_size_str, ' ');
			if (error_code) {
				return FAILURE;
			}

			printf("Answer Image Size: %s\n", image_size_str);

			error_code = write_from_file_to_socket(socket_tcp, question->answers[i]->image_path, question->answers[i]->image_size);
			if (error_code) {
				return FAILURE;
			}

			printf("Answer Image Path: %s\n", question->answers[i]->image_path);

		} else {
			error_code = write_to_tcp_socket(socket_tcp, "0\0", '\0');
			if (error_code) {
				return FAILURE;
			}
			printf("Answer Has no img: 0\n");
		}
		printf("END\n");
	}

	error_code = write_to_tcp_socket(socket_tcp, "\0", '\n');
	if (error_code) {
		return FAILURE;
	}

	// write response
	return SUCCESS;
}
