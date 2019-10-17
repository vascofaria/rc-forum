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
#include "tcp-manager.h"

#include "../file-manager/file-manager.h"

#include "../protocol.h"
#include "../constants.h"
#include "../exceptions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_input_ANS(int socket_tcp, char *topic, char *question_title, answer_t **answer) {
	int  i, error_code;
	char size_str[MAX_SIZE_STR+1], opt;

	char answer_user_id[USER_ID_SIZE+1];
	char answer_title[MAX_ANSWER_TITLE+1];
	int  answer_size = 0;
	int  answer_img_size = 0;
	char answer_img_ext[IMAGE_EXT_SIZE+1];

	char answer_path[MAX_PATH], image_path[MAX_PATH];

	strcpy(answer_path, TMP_PATH);
	create_dir(answer_path);

	error_code = read_from_tcp_socket(socket_tcp, answer_user_id, USER_ID_SIZE + 1, ' ');
	printf("%s\n", answer_user_id);
	if (error_code || strlen(answer_user_id) != USER_ID_SIZE) {
		return BAD_INPUT;
	}

	for (i = 0; i < USER_ID_SIZE; i++) {
		if (answer_user_id[i] < '0' || answer_user_id[i] > '9') {
			return BAD_INPUT;
		}
	}

	error_code = read_from_tcp_socket(socket_tcp, topic, MAX_TOPIC_TITLE + 1, ' ');
	if (error_code) {
		return BAD_INPUT;
	}
	printf("%s\n", topic);
	error_code = read_from_tcp_socket(socket_tcp, question_title, MAX_QUESTION_TITLE + 1, ' ');
	if (error_code) {
		return BAD_INPUT;
	}
	printf("%s\n", question_title);
	error_code = read_from_tcp_socket(socket_tcp, size_str, MAX_SIZE_STR + 1, ' ');
	if (error_code) {
		return BAD_INPUT;
	}
	printf("%s\n", size_str);
	for (i = 0; i < strlen(size_str); i++) {
		if (size_str[i] < '0' || size_str[i] > '9') {
			return BAD_INPUT;
		}
		answer_size += size_str[i] - '0';
		answer_size *= 10;
	}
	answer_size /= 10;

	if (answer_size == 0) {
		return BAD_INPUT;
	}

	strcpy(image_path, answer_path);
	strcat(answer_path, "/answer.txt\0");

	// READ FILE FROM SOCKET
	error_code = write_from_socket_to_file(socket_tcp, answer_path, answer_size);
	if (error_code) {
		return BAD_INPUT;
	}

	error_code = read_from_tcp_socket(socket_tcp, NULL, 0, ' ');
	if (error_code) {
		return BAD_INPUT;
	}

	error_code = read_from_tcp_socket(socket_tcp, &opt, 1, '\0');
	printf("%c\n", opt);
	if (error_code) {
		return BAD_INPUT;
	}

	if (opt == '1') {

		error_code = read_from_tcp_socket(socket_tcp, NULL, 0, ' ');
		if (error_code) {
			return BAD_INPUT;
		}

		error_code = read_from_tcp_socket(socket_tcp, answer_img_ext, IMAGE_EXT_SIZE + 1, ' ');
		if (error_code) {
			return BAD_INPUT;
		}

		strcat(image_path, "/answer.\0");
		strcat(image_path, answer_img_ext);

		error_code = read_from_tcp_socket(socket_tcp, size_str, MAX_SIZE_STR + 1, ' ');
		if (error_code) {
			return BAD_INPUT;
		}
		
		for (i = 0; i < strlen(size_str); i++) {
			if (size_str[i] < '0' || size_str[i] > '9') {
				return BAD_INPUT;
			}
			answer_img_size += size_str[i] - '0';
			answer_img_size *= 10;
		}
		answer_img_size /= 10;

		if (answer_img_size == 0) {
			return BAD_INPUT;
		}

		// READ IMG FILE FROM SOCKET
		error_code = write_from_socket_to_file(socket_tcp, image_path, answer_img_size);
		if (error_code) {
			return BAD_INPUT;
		}

		error_code = read_from_tcp_socket(socket_tcp, NULL, 0, '\n');
		if (error_code) {
			return BAD_INPUT;
		}

		// What if theres something next??

		*answer = new_answer(ANSWER_TITLE_PRE, answer_user_id, "-1", answer_size, answer_path, answer_img_size, answer_img_ext, image_path);
		return SUCCESS;

	} else if (opt == '0') {
		error_code = read_from_tcp_socket(socket_tcp, NULL, 0, '\n');
		if (error_code) {
			return BAD_INPUT;
		}

		// What if theres something next??

		*answer = new_answer(ANSWER_TITLE_PRE, answer_user_id, "-1", answer_size, answer_path, 0, NULL, NULL);
		return SUCCESS;
	}

	return BAD_INPUT;
}

int parse_output_ANR(int socket_tcp) {
	int error_code;
	error_code = write_to_tcp_socket(socket_tcp, "ANR OK\n\0", '\0');
	if (error_code) {
		return FAILURE;
	}
	return SUCCESS;
}

void parse_output_ERROR_ANR(int socket_tcp, int error_code) {
	/* check wich error and turn it into the respective protocol error */
	if (error_code == BAD_INPUT || error_code == FAILURE) {
		write_to_tcp_socket(socket_tcp, "ANR NOK\0", '\n');
	} else if (error_code == TOPIC_DOESNT_EXIST) {
		write_to_tcp_socket(socket_tcp, "ANR NOK\0", '\n');
	} else if (error_code == QUESTION_DOESNT_EXIST) {
		write_to_tcp_socket(socket_tcp, "ANR NOK\0", '\n');
	} else if (error_code == MAX_ANSWERS_REACHED) {
		write_to_tcp_socket(socket_tcp, "ANR FUL\0", '\n');
	}
}
