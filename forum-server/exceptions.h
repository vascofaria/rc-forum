/* _______________________________________________________________________________
 * »»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
 * IST - RC 2019/2020
 *
 * exceptions.h
 *
 * -------------------------------------------------------------------------------
 *
 * Made by: Bruno Meira (89421), Nikoletta Matsur (89513) and Vasco Faria (89559)
 *
 * «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««
 */

#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__

enum exceptions {
	SUCCESS,
	FAILURE,
	BAD_INPUT,
	TOPIC_DOESNT_EXIST,
	TOPIC_ALREADY_EXISTS,
	MAX_TOPICS_REACHED,
	QUESTION_DOESNT_EXIST,
	QUESTION_ALREADY_EXISTS,
	MAX_QUESTIONS_REACHED,
	ANSWER_DOESNT_EXIST,
	ANSWER_ALREADY_EXISTS,
	MAX_ANSWERS_REACHED
};

#endif
