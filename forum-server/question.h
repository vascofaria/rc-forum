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

#ifndef QUESTION_H
#define QUESTION_H

#define MAX_TITLE   10
#define MAX_ANSWERS 10

typedef struct question {
	char title[NAME];
	char **answers;
} question_t;


question_t* new_question(char title[MAX_TITLE], ) {
	question_t *q = (question_t*) malloc(sizeof(question_t));
	q->title = title;
	q->answers = (char**) malloc(sizeof(char[MAX_TITLE]) * MAX_ANSWERS);
	return q;
}

void delete_question(question_t* question) {
	free(question_ptr->answers);
	free(question_ptr);
}

#endif