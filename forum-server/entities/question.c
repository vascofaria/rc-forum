/* _______________________________________________________________________________
 * »»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
 * IST - RC 2019/2020
 *
 * question.c
 *
 * -------------------------------------------------------------------------------
 *
 * Made by: Bruno Meira (89421), Nikoletta Matsur (89513) and Vasco Faria (89559)
 *
 * «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "./answer.h"
#include "./question.h"
#include "../constants.h"

question_t *new_question(char title[MAX_TITLE], char user_id[USER_ID_SIZE+1], int data_size, char data[MAX_TXT_SIZE], int image_size, char *image_ext, char *image, answer_t **answers) {
	int i;
	question_t *q = (question_t*) malloc(sizeof(question_t));

	q->title = (char*) malloc(sizeof(char)*(strlen(title)*1));
	strcpy(q->title, title);

	q->user_id = (char*) malloc(sizeof(char)*(USER_ID_SIZE+1));
	strcpy(q->user_id, user_id);

	q->data_size = data_size;
	q->data = (char*) malloc(sizeof(char)*(data_size+1));
	strcpy(q->data, data);

	q->image_size = image_size;
	if (image_size) {
		q->image_ext = (char*) malloc(sizeof(char)*(4));
		strcpy(q->image_ext, image_ext);
		q->image = (char*) malloc(sizeof(char)*(image_size+1));
		for (i = 0; i < image_size; i++) {
			q->image[i] = image[i];
		}
		q->image[i] = '\0';

	} else {
		q->image = NULL;
	}

	if (answers) {
		for (i = 0; i < MAX_ANSWERS && answers[i] != NULL; i++) {
			q->answers[i] = answers[i];
		}
		for (; i < MAX_ANSWERS; i++) {
			q->answers[i] = NULL;
		}
	} else {
		for (i = 0; i < MAX_ANSWERS; i++) {
			q->answers[i] = NULL;
		}
	}

	return q;
}

void free_question(question_t* question_ptr) {
	int i;

	free(question_ptr->title);
	free(question_ptr->user_id);
	free(question_ptr->data);

	if (question_ptr->image) {
		free(question_ptr->image_ext);
		free(question_ptr->image);
	}

	for (i = 0; i < MAX_ANSWERS; i++) {
		free_answer(question_ptr->answers[i]);
	}

	free(question_ptr);
	question_ptr = NULL;
}
