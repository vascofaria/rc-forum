/* _______________________________________________________________________________
 * »»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
 * IST - RC 2019/2020
 *
 * usecases.h
 *
 * -------------------------------------------------------------------------------
 *
 * Made by: Bruno Meira (89421), Nikoletta Matsur (89513) and Vasco Faria (89559)
 *
 * «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««
 */

#ifndef __USECASES_H__
#define __USECASES_H__

#include "../entities/question.h"
#include "../entities/answer.h"

int register_user(char *userID);

int topic_list(char ***topic_list);

int propose_topic(char *user_id, char *topic);

int list_questions(char *topic, char ***questions_list);

int search_question(char *topic, char *question_name, question_t **question);

int question_submit(char *topic, question_t *question);

int answer_submit(char *topic, char *question_name, answer_t *answer);

#endif