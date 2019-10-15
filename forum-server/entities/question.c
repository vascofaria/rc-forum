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

question_t *new_question(char title[MAX_QUESTION_TITLE], char user_id[USER_ID_SIZE+1], int data_size, char *data_path, int image_size, char *image_ext, char *image_path, answer_t **answers) {
	int i;
	question_t *q = (question_t*) malloc(sizeof(question_t));

	q->title = (char*) malloc(sizeof(char)*(strlen(title)+1));
	strcpy(q->title, title);

	q->user_id = (char*) malloc(sizeof(char)*(USER_ID_SIZE+1));
	strcpy(q->user_id, user_id);

	q->data_size = data_size;


	q->data_path = (char*) malloc(sizeof(char)*(strlen(data_path)+1));
	strcpy(q->data_path, data_path);

	q->image_size = image_size;
	if (image_size) {
		q->image_ext = (char*) malloc(sizeof(char)*(IMAGE_EXT_SIZE+1));
		strcpy(q->image_ext, image_ext);

		q->image_path = (char*) malloc(sizeof(char)*(strlen(image_path)+1));
		strcpy(q->image_path, image_path);

	} else {
		q->image_path = NULL;
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
	free(question_ptr->data_path);

	if (question_ptr->image_path) {
		free(question_ptr->image_ext);
		free(question_ptr->image_path);
	}

	for (i = 0; i < MAX_ANSWERS; i++) {
		free_answer(question_ptr->answers[i]);
	}

	free(question_ptr);
	question_ptr = NULL;
}
