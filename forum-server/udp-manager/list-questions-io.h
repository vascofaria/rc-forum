/* _______________________________________________________________________________
 * »»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
 * IST - RC 2019/2020
 *
 * list-questions-io.h
 *
 * -------------------------------------------------------------------------------
 *
 * Made by: Bruno Meira (89421), Nikoletta Matsur (89513) and Vasco Faria (89559)
 *
 * «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««
 */

#ifndef  __LIST_QUESTIONS_IO_H__
#define  __LIST_QUESTIONS_IO_H__

int parse_input_LQU(char* request, char* topic);

char *parse_output_LQR(char* topic, char **questions_list);

char *parse_output_ERROR_LQR(int error_code);

#endif
