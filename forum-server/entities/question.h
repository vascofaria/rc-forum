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
#include "./answer.h"

typedef struct question {
	char *title;
	char *user_id;
	int  data_size;
	char *data;
	int  image_size;
	char *image_ext;
	char *image;
	answer_t *answers[MAX_ANSWERS];
} question_t;

question_t *new_question(char *title, char *user_id, int data_size, char *data, int image_size, char *image_ext, char *image, answer_t **answers);

void free_question(question_t* question_ptr);

#endif
