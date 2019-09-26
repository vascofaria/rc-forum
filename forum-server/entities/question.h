/* _______________________________________________________________________________
 * »»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
 * IST - RC 2019/2020
 *
 * question.h
 *
 * -------------------------------------------------------------------------------
 *
 * Made by: Bruno Meira (89421), Nikoletta Matsur (89513) and Vasco Faria (89559)
 *
 * «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««
 */

#ifndef __QUESTION_H__
#define __QUESTION_H__

#include "../constants.h"
#include "answer.h"

typedef struct question {
	char title[MAX_TITLE];
	char data[MAX_TXT_SIZE];
	char image[MAX_IMG_SIZE];
	answer_t *answers[MAX_ANSWERS];
} question_t;


question_t *new_question(char title[MAX_TITLE], char data[MAX_TXT_SIZE], char image[MAX_IMG_SIZE], answer_t **answers) {
	int i;
	question_t *q = (question_t*) malloc(sizeof(question_t));

	strcpy(q->title, title);
	strcpy(q->data, data);
	strcpy(q->image, image);

	for (i = 0; i < MAX_ANSWERS && ; i++) {
		q->answers[i] = answers[i];
	}
	for (; i < MAX_ANSWERS; i++) {
		q->answers[i] = NULL;
	}

	return q;
}

void free_question(question_t* question_ptr) {
	int i;

	for (i = 0; i < MAX_ANSWERS; i++) {
		free_answer(question_ptr->answers[i]);
	}

	free(question_ptr);
	question_ptr = NULL;
}

#endif
