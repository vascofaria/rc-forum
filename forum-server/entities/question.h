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
	char *title;
	char *data;
	char *image;
	answer_t *answers[MAX_ANSWERS];
} question_t;


question_t *new_question(char title[MAX_TITLE], int data_size, char data[MAX_TXT_SIZE], int image_size, char image[MAX_IMG_SIZE], answer_t **answers) {
	int i;
	question_t *q = (question_t*) malloc(sizeof(question_t));

	q->title = (char*) malloc(sizeof(char)*(strlen(title)*1));
	strcpy(q->title, title);

	q->data = (char*) malloc(sizeof(char)*(data_size+1));
	strcpy(q->data, data);

	if (image_size) {
		q->image = (char*) malloc(sizeof(char)*(image_size+1));
		strcpy(q->image, image);
	} else {
		q->image = NULL;
	}

	for (i = 0; i < MAX_ANSWERS && q->answers[i] != NULL; i++) {
		q->answers[i] = answers[i];
	}
	for (; i < MAX_ANSWERS; i++) {
		q->answers[i] = NULL;
	}

	return q;
}

void free_question(question_t* question_ptr) {
	int i;

	free(question_ptr->title);
	free(question_ptr->data);

	if (question_ptr->image) {
		free(question_ptr->image);
	}

	for (i = 0; i < MAX_ANSWERS; i++) {
		free_answer(question_ptr->answers[i]);
	}

	free(question_ptr);
	question_ptr = NULL;
}

#endif
