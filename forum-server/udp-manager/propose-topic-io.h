/* _______________________________________________________________________________
 * »»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
 * IST - RC 2019/2020
 *
 * propose-topic-io.h
 *
 * -------------------------------------------------------------------------------
 *
 * Made by: Bruno Meira (89421), Nikoletta Matsur (89513) and Vasco Faria (89559)
 *
 * «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««
 */


#ifndef  __PROPOSE_TOPIC_IO_H__
#define  __PROPOSE_TOPIC_IO_H__

int parse_input_PTP(char* request, char* user_id, char* topic);

char *parse_output_PTR();

char *parse_output_ERROR_PTR(int error_code);

#endif