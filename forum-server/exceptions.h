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
	TOPICS_FULL,
	QUESTION_DOESNT_EXIST,
	QUESTION_ALREADY_EXISTS
};

#endif
