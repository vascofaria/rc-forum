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
#include "tcp-manager.h"

#include "../protocol.h"
#include "../exceptions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_input_QUS(int socket_tcp, char *topic, question_t **question) {
	int  i, error_code;
	char size_str[MAX_SIZE_STR+1], opt;

	int  question_size = 0;
	char question_user_id[USER_ID_SIZE+1];
	char question_title[MAX_QUESTION_TITLE+1];
	char question_data[MAX_TXT_SIZE+1];
	int  question_img_size = 0;
	char question_img_ext[IMAGE_EXT_SIZE+1];
	char question_img_data[MAX_IMG_SIZE+1];

	error_code = read_from_tcp_socket(socket_tcp, question_user_id, USER_ID_SIZE + 1, ' ');
	if (error_code || strlen(question_user_id) != USER_ID_SIZE) {
		return BAD_INPUT;
	}

	for (i = 0; i < USER_ID_SIZE; i++) {
		if (question_user_id[i] < '0' || question_user_id[i] > '9') {
			return BAD_INPUT;
		}
	}

	error_code = read_from_tcp_socket(socket_tcp, topic, MAX_TOPIC_TITLE + 1, ' ');
	if (error_code) {
		return BAD_INPUT;
	}

	error_code = read_from_tcp_socket(socket_tcp, question_title, MAX_QUESTION_TITLE + 1, ' ');
	if (error_code) {
		return BAD_INPUT;
	}

	error_code = read_from_tcp_socket(socket_tcp, size_str, MAX_SIZE_STR + 1, ' ');
	if (error_code) {
		return BAD_INPUT;
	}

	for (i = 0; i < MAX_SIZE_STR; i++) {
		if (size_str[i] < '0' || size_str[i] > '9') {
			return BAD_INPUT;
		}
		question_size += size_str[i] - '0';
		question_size *= 10;
	}

	if (question_size == 0) {
		return BAD_INPUT;
	}
	question_size /= 10;

	// READ FILE FROM SOCKET

	//if (request[i++] != ' ' || j != question_size) {
	//	return BAD_INPUT;
	//}

	error_code = read_from_tcp_socket(socket_tcp, size_str, 1 + 1, ' ');
	if (error_code) {
		return BAD_INPUT;
	}

	opt = size_str[0];
	if (opt == '1') {

		error_code = read_from_tcp_socket(socket_tcp, question_img_ext, IMAGE_EXT_SIZE + 1, ' ');
		if (error_code) {
			return BAD_INPUT;
		}

		error_code = read_from_tcp_socket(socket_tcp, size_str, MAX_SIZE_STR + 1, ' ');
		if (error_code) {
			return BAD_INPUT;
		}
		for (i = 0; i < MAX_SIZE_STR; i++) {
			if (size_str[i] < '0' || size_str[i] > '9') {
				return BAD_INPUT;
			}
			question_img_size += size_str[i] - '0';
			question_img_size *= 10;
		}

		if (question_img_size == 0) {
			return BAD_INPUT;
		}
		question_img_size /= 10;

		// READ IMG FILE FROM SOCKET

		//if (request[i] != ' ' && request[i+1] != '\0') {
		//	return BAD_INPUT;
		//}

		// care \0 above
		*question = new_question(question_title, question_user_id, question_size, question_data, question_img_size, question_img_ext, question_img_data, NULL);
		return SUCCESS;

	} else if (opt == '0') {
		// if (request[i] != ' ' && request[i+1] != '\0') {
		// 	return BAD_INPUT;
		// }

		// care \0 above
		*question = new_question(question_title, question_user_id, question_size, question_data, 0, NULL, NULL, NULL);
		return SUCCESS;
	}

	return BAD_INPUT;
}


int parse_output_QUR(int socket_tcp) {
	int error_code;
	error_code = write_to_tcp_socket(socket_tcp, "OK\n\0", '\0');
	if (error_code) {
		return FAILURE;
	}
	return SUCCESS;
}
