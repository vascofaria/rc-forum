/* _______________________________________________________________________________
 * »»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
 * IST - RC 2019/2020
 *
 * answer.c
 *
 * -------------------------------------------------------------------------------
 *
 * Made by: Bruno Meira (89421), Nikoletta Matsur (89513) and Vasco Faria (89559)
 *
 * «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««
 */

#include <stdlib.h>
#include <string.h>

#include "./answer.h"
#include "../constants.h"

answer_t *new_answer(char title[MAX_ANSWER_TITLE+1], char user_id[USER_ID_SIZE+1], char number[MAX_NUM_STR+1], int data_size, char *data_path, int image_size, char *image_ext, char *image_path) {
	int title_size = strlen(title);
	answer_t *a = (answer_t*) malloc(sizeof(answer_t));

	a->title = (char*) malloc(sizeof(char)*(title_size));
	strcpy(a->title, title);

	a->user_id = (char*) malloc(sizeof(char)*(USER_ID_SIZE+1));
	strcpy(a->user_id, user_id);

	a->number = (char*) malloc(sizeof(char)*(MAX_NUM_STR+1));
	strcpy(a->number, number);

	a->data_size = data_size;
	a->data_path = (char*) malloc(sizeof(char)*(strlen(data_path)+1));
	strcpy(a->data_path, data_path);

	a->image_size = image_size;
	if (image_size) {
		a->image_ext = (char*) malloc(sizeof(char)*(IMAGE_EXT_SIZE+1));
		strcpy(a->image_ext, image_ext);

		a->image_path = (char*) malloc(sizeof(char)*(strlen(image_path)+1));
		strcpy(a->image_path, image_path);
	} else {
		a->image_path = NULL;
	}

	return a;
}

void free_answer(answer_t *answer_ptr) {

	free(answer_ptr->title);
	free(answer_ptr->user_id);
	free(answer_ptr->number);
	free(answer_ptr->data_path);

	if (answer_ptr->image_path) {
		free(answer_ptr->image_ext);
		free(answer_ptr->image_path);
	}

	free(answer_ptr);
	answer_ptr = NULL;
}