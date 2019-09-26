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

#include "../constants.h"

typedef struct answer {
	char title[MAX_TITLE];
	char data[MAX_TXT_SIZE];
	char image[MAX_IMG_SIZE];
} answer_t;

answer_t *new_answer(char title[MAX_TITLE], char data[MAX_TXT_SIZE], char image[MAX_IMG_SIZE]) {
	
	answer_t *a = (answer_t*) malloc(sizeof(answer_t));

	strcpy(a->title, title);
	strcpy(a->data, data);
	if (image) {
		strcpy(a->image, image);
	}

	return a;
}

void free_answer(answer_t *answer_ptr) {
	free(answer_ptr);
	answer_ptr = NULL;
}

#endif