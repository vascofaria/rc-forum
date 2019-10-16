/* _______________________________________________________________________________
 * »»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
 * IST - RC 2019/2020
 *
 * constants.h
 *
 * -------------------------------------------------------------------------------
 *
 * Made by: Bruno Meira (89421), Nikoletta Matsur (89513) and Vasco Faria (89559)
 *
 * «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««
 */

#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#define USER_ID_SIZE 5
#define MAX_NUM_STR  2

#define MAX_TOPIC_TITLE    10
#define MAX_QUESTION_TITLE 10
#define MAX_ANSWER_TITLE   10

#define MAX_TOPICS    99
#define MAX_QUESTIONS 99
#define MAX_ANSWERS   10

#define MAX_SIZE_STR   5
#define MAX_TXT_SIZE   256
#define IMAGE_EXT_SIZE 3
#define MAX_IMG_SIZE   5120

#define MAX_STATUS_RESPONSE 1000
#define MAX_TOPIC_LIST_RESPONSE 1100
//(10+1+5)*99 + 99*1

#define FALSE 0
#define TRUE  1

#define MAX_PATH      90
#define MAX_FILENAME  20

#define TMP_PATH       "./tmp/\0"
#define TOPICS_PATH    "./topics/\0"
#define QUESTIONS_PATH "/questions/\0"
#define ANSWERS_PATH   "/answers/\0"

#endif