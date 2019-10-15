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

#include "../protocol.h"
#include "../constants.h"
#include "../exceptions.h"

#include <stdlib.h>
#include <string.h>

int parse_input_ANS(int socket_tcp, char *topic, char *question_title, answer_t **answer) {
	int  i, error_code;
	char size_str[MAX_SIZE_STR+1], opt;

	char answer_user_id[USER_ID_SIZE+1];
	char answer_title[MAX_ANSWER_TITLE+1];
	int  answer_size = 0;
	char answer_data[MAX_TXT_SIZE+1];
	int  answer_img_size = 0;
	char answer_img_ext[IMAGE_EXT_SIZE+1];
	char answer_img_data[MAX_IMG_SIZE+1];

	error_code = read_from_tcp_socket(socket_tcp, answer_user_id, USER_ID_SIZE + 1, ' ');
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
		answer_size += size_str[i] - '0';
		answer_size *= 10;
	}
	if (answer_size == 0) {
		return BAD_INPUT;
	}
	answer_size /= 10;

	// READ FILE FROM SOCKET

	//if (request[i++] != ' ' || j != answer_size) {
	//	return BAD_INPUT;
	//}

	error_code = read_from_tcp_socket(socket_tcp, size_str, 1 + 1, ' ');
	if (error_code) {
		return BAD_INPUT;
	}

	opt = size_str[0];
	if (opt == '1') {

		error_code = read_from_tcp_socket(socket_tcp, answer_img_ext, IMAGE_EXT_SIZE + 1, ' ');
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
			answer_img_size += size_str[i] - '0';
			answer_img_size *= 10;
		}
		if (answer_img_size == 0) {
			return BAD_INPUT;
		}
		answer_img_size /= 10;

		// READ IMG FILE FROM SOCKET

		//if (request[i] != ' ' && request[i+1] != '\0') {
		//	return BAD_INPUT;
		//}

		// care \0 above
		*answer = new_answer(answer_title, answer_user_id, answer_size, answer_data, answer_img_size, answer_img_ext, answer_img_data);
		return SUCCESS;

	} else if (opt == '0') {
		//if (request[i] != ' ' && request[i+1] != '\0') {
		//	return BAD_INPUT;
		//}

		// care \0 above
		*answer = new_answer(answer_title, answer_user_id, answer_size, answer_data, 0, NULL, NULL);
		return SUCCESS;
	}

	return BAD_INPUT;
}

int parse_output_ANR(int socket_tcp) {
	int error_code;
	error_code = write_to_tcp_socket(socket_tcp, "OK\n\0", '\0');
	if (error_code) {
		return FAILURE;
	}
	return SUCCESS;
}