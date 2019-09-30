/* _______________________________________________________________________________
 * »»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
 * IST - RC 2019/2020
 *
 * answer.h
 *
 * -------------------------------------------------------------------------------
 *
 * Made by: Bruno Meira (89421), Nikoletta Matsur (89513) and Vasco Faria (89559)
 *
 * «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««
 */

#ifndef __ANSWER_H__
#define __ANSWER_H__

#include <stdlib.h>
#include <string.h>
#include "../constants.h"


typedef struct answer {
	char *title;
	char *data;
	char *image;
} answer_t;

answer_t *new_answer(char title[MAX_TITLE], int data_size, char data[MAX_TXT_SIZE], int image_size, char image[MAX_IMG_SIZE]) {
	int title_size = strlen(title);
	answer_t *a = (answer_t*) malloc(sizeof(answer_t));

	a->title = (char*) malloc(sizeof(char)*(title_size));
	strcpy(a->title, title);

	a->data = (char*) malloc(sizeof(char)*(data_size));
	strcpy(a->data, data);


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

#endif