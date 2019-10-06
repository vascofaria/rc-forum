/* _______________________________________________________________________________
 * »»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
 * IST - RC 2019/2020
 *
 * file-manager.h
 *
 * -------------------------------------------------------------------------------
 *
 * Made by: Bruno Meira (89421), Nikoletta Matsur (89513) and Vasco Faria (89559)
 *
 * «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««
 */

#ifndef  __FILE_MANAGER_H__
#define  __FILE_MANAGER_H__

#include "../exceptions.h"
#include "../entities/question.h"
#include "../entities/answer.h"

/* ===============================================================================
 * int topic_exists(char *topic_path);
 * -------------------------------------------------------------------------------
 * 
 * -------------------------------------------------------------------------------
 * RETURN TOPIC_DOESNT_EXIST if topic doesnt exist
 * RETURN TOPIC_ALREADY_EXISTS if topic already exist
 * ===============================================================================
*/
int topic_exists(char *topic_path);


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
 * char **list_directory(char* path);
 * -------------------------------------------------------------------------------
 * Lists all the files ignoring the ones that start with '.'
 * -------------------------------------------------------------------------------
 * RETURN a pointer to an allocated strings list
 * ===============================================================================
*/
// static char **list_directory(char* path);


/* ===============================================================================
 * int get_topics(char ***topics_list);
 * -------------------------------------------------------------------------------
 * 
 * -------------------------------------------------------------------------------
 * RETURN SUCCESS if success
 * ===============================================================================
*/
int get_topics(char ***topics_list);

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
 * int get_answers(char *topic_name, char *question_name, char ***answers_list);
 * -------------------------------------------------------------------------------
 * 
 * -------------------------------------------------------------------------------
 * RETURN SUCCESS if success
 * RETURN TOPIC_DOESNT_EXIST if question doesnt exist
 * RETURN QUESTION_DOESNT_EXIST if question already exist
 * ===============================================================================
*/
int get_answers(char *topic_name, char *question_name, char ***answers_list);


/* ===============================================================================
 * int post_topic(char *user_id, char *topic_name);
 * -------------------------------------------------------------------------------
 * 
 * -------------------------------------------------------------------------------
 * RETURN SUCCESS if success
 * RETURN TOPIC_ALREADY_EXISTS if topic already exist
 * ===============================================================================
*/
int post_topic(char *user_id, char *topic_name);

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