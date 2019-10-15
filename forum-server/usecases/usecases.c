/* _______________________________________________________________________________
 * »»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
 * IST - RC 2019/2020
 *
 * usecases.c
 *
 * -------------------------------------------------------------------------------
 *
 * Made by: Bruno Meira (89421), Nikoletta Matsur (89513) and Vasco Faria (89559)
 *
 * «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "usecases.h"
#include "../exceptions.h"
#include "../file-manager/file-manager.h"

#include "../file-manager/topic-file-manager.h"
#include "../file-manager/question-file-manager.h"
#include "../file-manager/answer-file-manager.h"

int register_user(char *userID) {
    return SUCCESS;
}

int topic_list(char ***topic_list) {
    int error_code;

    error_code = get_topics(topic_list);

    return error_code;
}

int propose_topic(char *user_id, char *topic) {
    int error_code;

    error_code = post_topic(user_id, topic);

    return error_code;
}

int list_questions(char *topic, char ***questions_list) {
    int error_code;

    error_code = get_questions(topic, questions_list);

    return error_code;
}

int search_question(char *topic, char *question_name, question_t **question) {
    int error_code;

    error_code = get_question(topic, question_name, question);

    return error_code;
}

int question_submit(char *topic, question_t *question) {
    int error_code;

    error_code = post_question(topic, question);

    return error_code;
}

int answer_submit(char *topic, char *question_name, answer_t *answer) {
    int error_code;

    error_code = post_answer(topic, question_name, answer);

    return error_code;
}
