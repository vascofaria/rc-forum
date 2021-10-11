/* _______________________________________________________________________________
 * »»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
 * IST - RC 2019/2020
 *
 * question-get-io.h
 *
 * -------------------------------------------------------------------------------
 *
 * Made by: Bruno Meira (89421), Nikoletta Matsur (89513) and Vasco Faria (89559)
 *
 * «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««
 */

#ifndef  __QUESTION_GET_IO_H__
#define  __QUESTION_GET_IO_H__

#include "../entities/question.h"

int parse_input_GQU(int socket_tcp, char *topic, char *question_title);

int parse_output_QGR(int socket_tcp, question_t *question);

void parse_output_ERROR_QGR(int socket_tcp, int error_code);

#endif