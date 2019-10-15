/* _______________________________________________________________________________
 * »»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
 * IST - RC 2019/2020
 *
 * tcp-manager.h
 *
 * -------------------------------------------------------------------------------
 *
 * Made by: Bruno Meira (89421), Nikoletta Matsur (89513) and Vasco Faria (89559)
 *
 * «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««
 */

#ifndef __TCP_MANAGER_H__
#define __TCP_MANAGER_H__


int write_to_tcp_socket(int socket_tcp, char *buffer, char final_char);

int read_from_tcp_socket(int socket_tcp, char *buffer, int size, char final_char);

/*
 * RETURNS A RESPONSE
*/
void tcp_manager(int socket_tcp);


#endif
