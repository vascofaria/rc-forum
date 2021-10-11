/* _______________________________________________________________________________
 * »»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
 * IST - RC 2019/2020
 *
 * question-file-manager.h
 *
 * -------------------------------------------------------------------------------
 *
 * Made by: Bruno Meira (89421), Nikoletta Matsur (89513) and Vasco Faria (89559)
 *
 * «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««
 */

#ifndef  __QUESTION_FILE_MANAGER_H__
#define  __QUESTION_FILE_MANAGER_H__

#include "../entities/answer.h"
#include "../entities/question.h"

/* ===============================================================================
 * int question_exists(char *question_path);
 * -------------------------------------------------------------------------------
 * 
 * -------------------------------------------------------------------------------
 * RETURN QUESTION_DOESNT_EXIST if question doesnt exist
 * RETURN QUESTION_ALREADY_EXISTS if question already exist
 * ===============================================================================
*/
int question_exists(char *question_path);

/* ===============================================================================
 * int get_question(char *topic_name, question_title, question_t **question);
 * -------------------------------------------------------------------------------
 * 
 * -------------------------------------------------------------------------------
 * RETURN SUCCESS if success
 * RETURN TOPIC_DOESNT_EXIST or QUESTION_DOESNT_EXIST if question doesnt exist
 * ===============================================================================
*/
int get_question(char *topic_name, char *question_title, question_t **question);

/* ===============================================================================
 * int get_questions(char *topic_name, char ***questions_list);
 * -------------------------------------------------------------------------------
 * 
 * -------------------------------------------------------------------------------
 * RETURN SUCCESS if success
 * RETURN TOPIC_DOESNT_EXIST if question doesnt exist
 * ===============================================================================
*/
int get_questions(char *topic_name, char ***questions_list);

/* ===============================================================================
 * int post_question(char *user_id, char *topic_name, question_t *question);
 * -------------------------------------------------------------------------------
 * 
 * -------------------------------------------------------------------------------
 * RETURN SUCCESS if success
 * RETURN TOPIC_ALREADY_EXISTS if question already exists
 * ===============================================================================
*/
int post_question(char *topic_name, question_t *question);

#endif
