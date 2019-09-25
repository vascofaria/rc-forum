#include <stdio.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 128
#define ASCII_LIMIT 256
#define MAX_ARGS_N  3
#define MAX_ARGS_L  128

enum param_types {
	PARAM_FSIP    = (unsigned char)'n',
	PARAM_FSPORT  = (unsigned char)'p',
};

static void set_default_params(char main_params[ASCII_LIMIT][BUFFER_SIZE]) {
	strcpy(main_params[PARAM_FSIP],   "xsy");
	strcpy(main_params[PARAM_FSPORT], "58000");
}

static void concatenate_args(char *buffer, char* msg, char args[MAX_ARGS_N][MAX_ARGS_L], int num_tokens) {
	int i;
	buffer[0] = '\0';
	strcpy(buffer, msg);
	for (i = 0; i < num_tokens - 1; i++) {
		strcat(buffer, " ");
		strcat(buffer, args[i + 1]);
	}
	strcat(buffer, "\n\0");
}

static void executes_tcp_command(char *buffer, struct addrinfo *res_tcp) {
	int server_sock_tcp, error_code;

	/* Socket TCP creation */
	server_sock_tcp = socket(res_tcp->ai_family, res_tcp->ai_socktype, res_tcp->ai_protocol);

	if (server_sock_tcp == -1) {
		fprintf(stderr, "socket creation failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	/*			           */

	/* Connect to TCP socket */
	error_code = connect(server_sock_tcp, res_tcp->ai_addr, res_tcp->ai_addrlen);

	if (error_code == -1) {
		fprintf(stderr, "connect failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* Write on server socket */
	write(server_sock_tcp, buffer, BUFFER_SIZE);

	/* Read from server socket */
	read(server_sock_tcp, buffer, BUFFER_SIZE);

	error_code = close(server_sock_tcp);
	if (error_code == -1) {
		fprintf(stderr, "close failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char const *argv[])
{
	int 				server_sock_udp, error_code, num_tokens;
	struct addrinfo 	hints, *res_udp, *res_tcp;
	char				buffer[BUFFER_SIZE], *buffer_ptr, main_params[ASCII_LIMIT][BUFFER_SIZE], args[MAX_ARGS_N][MAX_ARGS_L];
	ssize_t 			n;

	set_default_params(main_params);

	/* Initialization of hints structure for socket UDP */
	memset((void *) &hints, 0, sizeof(struct addrinfo));
	hints.ai_family   = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags    = AI_NUMERICSERV;
	/*									                */

	error_code = getaddrinfo(main_params[PARAM_FSIP], main_params[PARAM_FSPORT], &hints, &res_udp);

	if (error_code != 0) {
		fprintf(stderr, "getaddrinfo failed: %s\n", gai_strerror(error_code));
		exit(EXIT_FAILURE);
	}

	/* Socket UDP creation */
	server_sock_udp = socket(res_udp->ai_family, res_udp->ai_socktype, res_udp->ai_protocol);

	if (server_sock_udp == -1) {
		fprintf(stderr, "socket creation failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	/*			           */

	/* Initialization of hints structure for socket TCP */
	hints.ai_socktype = SOCK_STREAM;
	/*                                                  */
		
	error_code = getaddrinfo(main_params[PARAM_FSIP], main_params[PARAM_FSPORT], &hints, &res_tcp);

	if (error_code != 0) {
		fprintf(stderr, "getaddrinfo failed: %s\n", gai_strerror(error_code));
		exit(EXIT_FAILURE);
	}


	while (1) {
		printf("Enter command (? for help) : ");

		/* reads from terminal */
		buffer_ptr = fgets(buffer, BUFFER_SIZE, stdin);
		if (buffer_ptr == NULL) {
			fprintf(stderr, "fgets failed\n");
			exit(EXIT_FAILURE);
		}
		/*                     */

		/* executes command */
		num_tokens = sscanf(buffer, "%s %s %s %s %s", args[0], args[1], args[2], args[3], args[4]);

		if (!strcmp(args[0], "register")             || 
			!strcmp(args[0], "reg")) {
				if (num_tokens == 2) {
					concatenate_args(buffer, "REG", args, num_tokens);
					sendto(server_sock_udp, buffer, strlen(buffer), 0, (struct sockaddr *) res_udp->ai_addr, res_udp->ai_addrlen);
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
		else if (!strcmp(args[0], "exit")) {
				if (num_tokens == 1) {
					break;
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
		else {
				fprintf(stdout, "Invalid command!\n");
		}
		/*                  */
	}

	error_code = close(server_sock_udp);
	if (error_code == -1) {
		fprintf(stderr, "close failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	return 0;
}