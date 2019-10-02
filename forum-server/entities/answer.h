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


typedef struct answer {
	char *title;
	int  data_size;
	char *data;
	int  image_size;
	char *image;
} answer_t;

answer_t *new_answer(char *title, int data_size, char *data, int image_size, char *image);

void free_answer(answer_t *answer_ptr);

#endif