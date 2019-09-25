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

#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#define SUCCESS_CODE            0
#define TOPIC_DOESNT_EXIST      1
#define TOPIC_ALREADY_EXISTS    2
#define QUESTION_DOESNT_EXIST   3
#define QUESTION_ALREADY_EXISTS 4


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
char **list_directory(char* path);



/* ===============================================================================
 * int get_topics(char ***topics_list);
 * -------------------------------------------------------------------------------
 * 
 * -------------------------------------------------------------------------------
 * RETURN SUCCESS_CODE if success
 * ===============================================================================
*/
int get_topics(char ***topics_list);

/* ===============================================================================
 * int get_questions(char *topic_name, char ***questions_list);
 * -------------------------------------------------------------------------------
 * 
 * -------------------------------------------------------------------------------
 * RETURN SUCCESS_CODE if success
 * RETURN TOPIC_DOESNT_EXIST if question doesnt exist
 * ===============================================================================
*/
int get_questions(char *topic_name, char ***questions_list);

/* ===============================================================================
 * int get_answers(char *topic_name, char *question_name, char ***answers_list);
 * -------------------------------------------------------------------------------
 * 
 * -------------------------------------------------------------------------------
 * RETURN SUCCESS_CODE if success
 * RETURN TOPIC_DOESNT_EXIST if question doesnt exist
 * RETURN QUESTION_DOESNT_EXIST if question already exist
 * ===============================================================================
*/
int get_answers(char *topic_name, char *question_name, char ***answers_list);


/* ===============================================================================
 * int post_topic(char *topic_name, char *user_id);
 * -------------------------------------------------------------------------------
 * 
 * -------------------------------------------------------------------------------
 * RETURN SUCCESS_CODE if success
 * RETURN TOPIC_ALREADY_EXISTS if topic already exist
 * ===============================================================================
*/
int post_topic(char *topic_name, char *user_id);


#endif