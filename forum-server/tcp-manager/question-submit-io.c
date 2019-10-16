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

#include "../file-manager/file-manager.h"

#include "../protocol.h"
#include "../exceptions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int parse_input_QUS(int socket_tcp, char *topic, question_t **question) {
	int  i, error_code;
	char size_str[MAX_SIZE_STR+1], opt;

	int  question_size = 0;
	char question_user_id[USER_ID_SIZE+1];
	char question_title[MAX_QUESTION_TITLE+1];
	int  question_img_size = 0;
	char question_img_ext[IMAGE_EXT_SIZE+1];

	char question_path[MAX_PATH], image_path[MAX_PATH];

	strcpy(question_path, TMP_PATH);
	create_dir(question_path);

	error_code = read_from_tcp_socket(socket_tcp, question_user_id, USER_ID_SIZE + 1, ' ');
	if (error_code || strlen(question_user_id) != USER_ID_SIZE) {
		return BAD_INPUT;
	}

	strcat(question_path, question_user_id);
	create_dir(question_path);

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

	for (i = 0; i < strlen(size_str); i++) {
		if (size_str[i] < '0' || size_str[i] > '9') {
			return BAD_INPUT;
		}
		question_size += size_str[i] - '0';
		question_size *= 10;
	}
	question_size /= 10;

	if (question_size == 0) {
		return BAD_INPUT;
	}

	strcpy(image_path, question_path);
	strcat(question_path, "/question.txt\0");

	// READ FILE FROM SOCKET
	error_code = write_from_socket_to_file(socket_tcp, question_path, question_size);
	if (error_code) {
		return BAD_INPUT;
	}

	error_code = read_from_tcp_socket(socket_tcp, NULL, 0, ' ');
	if (error_code) {
		return BAD_INPUT;
	}

	error_code = read_from_tcp_socket(socket_tcp, &opt, 1, '\0');
	if (error_code) {
		return BAD_INPUT;
	}

	if (opt == '1') {

		error_code = read_from_tcp_socket(socket_tcp, NULL, 0, ' ');
		if (error_code) {
			return BAD_INPUT;
		}

		error_code = read_from_tcp_socket(socket_tcp, question_img_ext, IMAGE_EXT_SIZE + 1, ' ');
		if (error_code) {
			return BAD_INPUT;
		}

		strcat(image_path, "/question.\0");
		strcat(image_path, question_img_ext);

		error_code = read_from_tcp_socket(socket_tcp, size_str, MAX_SIZE_STR + 1, ' ');
		if (error_code) {
			return BAD_INPUT;
		}
		for (i = 0; i < strlen(size_str); i++) {
			if (size_str[i] < '0' || size_str[i] > '9') {
				printf("%s\n", size_str[i]);
				return BAD_INPUT;
			}
			question_img_size += size_str[i] - '0';
			question_img_size *= 10;
		}
		question_img_size /= 10;

		if (question_img_size == 0) {
			return BAD_INPUT;
		}

		// READ IMG FILE FROM SOCKET
		error_code = write_from_socket_to_file(socket_tcp, image_path, question_img_size);
		if (error_code) {
			return BAD_INPUT;
		}

		error_code = read_from_tcp_socket(socket_tcp, NULL, 0, '\n');
		if (error_code) {
			return BAD_INPUT;
		}

		// What if theres something next??

		*question = new_question(question_title, question_user_id, question_size, question_path, question_img_size, question_img_ext, image_path, NULL);
		return SUCCESS;

	} else if (opt == '0') {

		error_code = read_from_tcp_socket(socket_tcp, NULL, 0, '\n');
		if (error_code) {
			return BAD_INPUT;
		}

		// What if theres something next??
		
		*question = new_question(question_title, question_user_id, question_size, question_path, 0, NULL, NULL, NULL);
		return SUCCESS;
	}

	return BAD_INPUT;
}


int parse_output_QUR(int socket_tcp) {
	int error_code;
	error_code = write_to_tcp_socket(socket_tcp, "QUR OK\n\0", '\0');
	if (error_code) {
		return FAILURE;
	}
	return SUCCESS;
}
