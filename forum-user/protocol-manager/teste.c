#include "client-manager.h"

#define MAX_ARGS_N   3
#define MAX_ARGS_L   128

char*
client_manager(char *request) {
	char* new_request;
	char* args[MAX_ARGS_N][MAX_ARGS_L];

	num_tokens = sscanf(buffer, "%s %s %s %s %s", args[0], args[1], args[2], args[3], args[4]);

	if (!strcmp(args[0], "register")             || 
		!strcmp(args[0], "reg")) {
			if (num_tokens == 2) {
				client_udp_manager()
			}
			else {
				fprintf(stderr, "Invalid number of parameters: %d\n", num_tokens);
			}
	}
	else if (!strcmp(args[0], "topic_list")      || 
		     !strcmp(args[0], "tl")) {
			if (num_tokens == 1) {
				concatenate_args(buffer, "LTP", args, num_tokens);
				sendto(server_sock_udp, buffer, strlen(buffer), 0, (struct sockaddr *) res_udp->ai_addr, res_udp->ai_addrlen);
			}
			else {
				fprintf(stderr, "Invalid number of parameters: %d\n", num_tokens);
			}
	}
	else if (!strcmp(args[0], "topic_select")    || 
		     !strcmp(args[0], "ts")) {
			 /* this option dont comunicate with server */
			if (num_tokens != 1) {
				fprintf(stderr, "Invalid number of parameters: %d\n", num_tokens);
			} 
	}
	else if (!strcmp(args[0], "topic_propose")   || 
		     !strcmp(args[0], "tp")) {
			if (num_tokens == 2) {
				concatenate_args(buffer, "PTP", args, num_tokens);
				sendto(server_sock_udp, buffer, strlen(buffer), 0, (struct sockaddr *) res_udp->ai_addr, res_udp->ai_addrlen);
			}
			else {
				fprintf(stderr, "Invalid number of parameters: %d\n", num_tokens);
			}
	}
	else if (!strcmp(args[0], "question_list")   || 
		     !strcmp(args[0], "ql")) {
			if (num_tokens == 1) {
				concatenate_args(buffer, "LQU", args, num_tokens);
				sendto(server_sock_udp, buffer, strlen(buffer), 0, (struct sockaddr *) res_udp->ai_addr, res_udp->ai_addrlen);
			}
			else {
				fprintf(stderr, "Invalid number of parameters: %d\n", num_tokens);
			}
	}
	else if (!strcmp(args[0], "question_get")    || 
		     !strcmp(args[0], "qg")) {
			if (num_tokens == 2) {
				concatenate_args(buffer, "GQU", args, num_tokens);
				executes_tcp_command(buffer, res_tcp);
			}
			else {
				fprintf(stderr, "Invalid number of parameters: %d\n", num_tokens);
			}
	}
	else if (!strcmp(args[0], "question_submit") || 
		     !strcmp(args[0], "qs")) {
			if (num_tokens == 3 || num_tokens == 4) {
				concatenate_args(buffer, "QUS", args, num_tokens);
				executes_tcp_command(buffer, res_tcp);
			}
			else {
				fprintf(stderr, "Invalid number of parameters: %d\n", num_tokens);
			}
	}
	else if (!strcmp(args[0], "answer_submit")   || 
		     !strcmp(args[0], "as")) {
			if (num_tokens == 2 || num_tokens == 3) {	
				concatenate_args(buffer, "ANS", args, num_tokens);
				executes_tcp_command(buffer, res_tcp);
			}
			else {
				fprintf(stderr, "Invalid number of parameters: %d\n", num_tokens);
			}
	}
	else if (!strcmp(args[0], "?")) {
			if (num_tokens == 1) {
				printf( "\tregister\t/reg\t<userId>\n" 
						"\ttopic_list\t/tl\n"
						"\ttopic_select\t\t<topic name>\n"
						"\tts\t\t\t<topic number>\n"
						"\ttopic_propose\t/tp\t<topic>\n"
						"\tquestion_list\t/ql\n"
						"\tquestion_get\t\t<question name>\n"
						"\tqg\t\t\t<question_number>\n"
						"\tanswer_submit\t/as\t<text_file [image_file]>\n"
						"\tquestion_submit\t/qs\t<text_file [image_file]>\n"
						"\texit\n");
			}
			else {
				fprintf(stderr, "Invalid number of parameters: %d\n", num_tokens);
			}
	}
	else if (!strcmp(args[0], "exit")) {
			if (num_tokens == 1) {
				break;
			}
			else {
				fprintf(stderr, "Invalid number of parameters: %d\n", num_tokens);
			}

	}
	else {
			fprintf(stdout, "Invalid command!\n");
	}
}