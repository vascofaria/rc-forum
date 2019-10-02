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

answer_t *new_answer(char title[MAX_TITLE], int data_size, char data[MAX_TXT_SIZE], int image_size, char image[MAX_IMG_SIZE]) {
	int title_size = strlen(title);
	answer_t *a = (answer_t*) malloc(sizeof(answer_t));

	a->title = (char*) malloc(sizeof(char)*(title_size));
	strcpy(a->title, title);

	a->data_size = data_size;
	a->data = (char*) malloc(sizeof(char)*(data_size));
	strcpy(a->data, data);

	a->image_size = image_size;
	if (image_size) {
		a->image = (char*) malloc(sizeof(char)*(image_size));
		strcpy(a->image, image);
	} else {
		a->image = NULL;
	}

	return a;
}

void free_answer(answer_t *answer_ptr) {

	free(answer_ptr->title);
	free(answer_ptr->data);

	if (answer_ptr->image) {
		free(answer_ptr->image);
	}

	free(answer_ptr);
	answer_ptr = NULL;
}