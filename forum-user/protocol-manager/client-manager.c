#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client-manager.h"
#include "client-udp-manager.h"
#include "client-tcp-manager.h"

void
client_manager(user_t *user, char *request) {
	char args[MAX_ARGS_N][MAX_ARGS_L];
	int  num_tokens;

	num_tokens = sscanf(request, "%s %s %s %s %s", args[0], args[1], args[2], args[3], args[4]);

	if (!strcmp(args[0], "register")             || 
		!strcmp(args[0], "reg")) {
			if (num_tokens == 2) {
				client_udp_manager(user, "REG", args);
			}
			else {
				fprintf(stderr, "Invalid number of parameters: %d\n", num_tokens);
			}
	}
	else if (!strcmp(args[0], "topic_list")      || 
		     !strcmp(args[0], "tl")) {
			if (num_tokens == 1) {
				client_udp_manager(user, "LTP", args);
			}
			else {
				fprintf(stderr, "Invalid number of parameters: %d\n", num_tokens);
			}
	}
	else if (!strcmp(args[0], "topic_select")    || 
		     !strcmp(args[0], "ts")) {
			 /* this option dont comunicate with server */
			if (num_tokens != 2) {
				fprintf(stderr, "Invalid number of parameters: %d\n", num_tokens);
			}
			else {
				set_user_topic(user, args[1]);
			} 
	}
	else if (!strcmp(args[0], "topic_propose")   || 
		     !strcmp(args[0], "tp")) {
			if (num_tokens == 2) {
				client_udp_manager(user, "PTP", args);
			}
			else {
				fprintf(stderr, "Invalid number of parameters: %d\n", num_tokens);
			}
	}
	else if (!strcmp(args[0], "question_list")   || 
		     !strcmp(args[0], "ql")) {
			if (num_tokens == 1) {
				client_udp_manager(user, "LQU", args);
			}
			else {
				fprintf(stderr, "Invalid number of parameters: %d\n", num_tokens);
			}
	}
	else if (!strcmp(args[0], "question_get")    || 
		     !strcmp(args[0], "qg")) {
			if (num_tokens == 2) {
				client_tcp_manager(user, "GQU", args, num_tokens);
			}
			else {
				fprintf(stderr, "Invalid number of parameters: %d\n", num_tokens);
			}
	}
	else if (!strcmp(args[0], "question_submit") || 
		     !strcmp(args[0], "qs")) {
			if (num_tokens == 3 || num_tokens == 4) {
				client_tcp_manager(user, "QUS", args, num_tokens);
			}
			else {
				fprintf(stderr, "Invalid number of parameters: %d\n", num_tokens);
			}
	}
	else if (!strcmp(args[0], "answer_submit")   || 
		     !strcmp(args[0], "as")) {
			if (num_tokens == 2 || num_tokens == 3) {	
				client_tcp_manager(user, "ANS", args, num_tokens);
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
				
			}
			else {
				fprintf(stderr, "Invalid number of parameters: %d\n", num_tokens);
			}

	}
	else {
			fprintf(stdout, "Invalid command!\n");
	}
}