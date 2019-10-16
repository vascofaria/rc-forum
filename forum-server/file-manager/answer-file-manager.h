/* _______________________________________________________________________________
 * »»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
 * IST - RC 2019/2020
 *
 * answer-file-manager.h
 *
 * -------------------------------------------------------------------------------
 *
 * Made by: Bruno Meira (89421), Nikoletta Matsur (89513) and Vasco Faria (89559)
 *
 * «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««
 */

#ifndef  __ANSWER_FILE_MANAGER_H__
#define  __ANSWER_FILE_MANAGER_H__

#include "../entities/answer.h"
#include "../entities/question.h"

/* ===============================================================================
 * int answer_exists(char *question_path);
 * -------------------------------------------------------------------------------
 * 
 * -------------------------------------------------------------------------------
 * RETURN ANSWER_DOESNT_EXIST if answer doesnt exist
 * RETURN ANSWER_ALREADY_EXISTS if answer already exist
 * ===============================================================================
*/
int answer_exists(char *answer_path);

/* ===============================================================================
 * int get_answers(char *topic_name, char *question_name, char ***answers_list);
 * -------------------------------------------------------------------------------
 * 
 * -------------------------------------------------------------------------------
 * RETURN SUCCESS if success
 * RETURN TOPIC_DOESNT_EXIST if question doesnt exist
 * RETURN QUESTION_DOESNT_EXIST if question already exist
 * ===============================================================================
*/
int get_answers(char *topic_name, char *question_name, answer_t ***answers_list);

/* ===============================================================================
 * int post_answer(char *user_id, char *topic_name, char *question_name, answer_t *answer);
 * -------------------------------------------------------------------------------
 * 
 * -------------------------------------------------------------------------------
 * RETURN SUCCESS if success
 * RETURN ANSWER_ALREADY_EXISTS if answer already exists
 * ===============================================================================
*/
int post_answer(char *topic_name, char *question_name, answer_t *answer);

#endif
