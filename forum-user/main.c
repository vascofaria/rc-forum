#include <stdio.h>
#include <netdb.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "protocol-manager/client-manager.h"
#include "user/user.h"

#define PORT 		 "58001"
#define BUFFER_SIZE  128
#define ASCII_LIMIT  256

enum param_types {
	PARAM_FSIP    = (unsigned char)'n',
	PARAM_FSPORT  = (unsigned char)'p',
};

static void set_default_params(char main_params[ASCII_LIMIT][BUFFER_SIZE]) {
	if(gethostname(main_params[PARAM_FSIP], BUFFER_SIZE) == -1) {
		fprintf(stderr, "gethostname failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	strcpy(main_params[PARAM_FSPORT], PORT);
}

static void display_usage (const char* appName, char main_params[ASCII_LIMIT][BUFFER_SIZE]) {
    printf("Usage: %s [options]\n", appName);
    puts("\nOptions:                            (defaults)\n");
    printf("    p <INT>    [p]ort               (%s)\n", main_params['p']);
    printf("    n <INT>    [n]ode               (%s)\n", main_params['n']);
    exit(EXIT_FAILURE);
}

static void parse_args (int argc, char* const argv[], char main_params[ASCII_LIMIT][BUFFER_SIZE]) {
    int i;
    int opt;

    opterr = 0;

    set_default_params(main_params);

    while ((opt = getopt(argc, argv, "p:n:")) != -1) {
        switch (opt) {
            case 'p':
            	strcpy(main_params[PARAM_FSPORT], optarg);
                break;
			case 'n':
            	strcpy(main_params[PARAM_FSIP], optarg);
                break;
			case '?':
            default:
                opterr++;
                break;
        }
    }

    for (i = optind; i < argc; i++) {
        fprintf(stderr, "Non-option argument: %s\n", argv[i]);
        opterr++;
    }

    if (opterr) {
        display_usage(argv[0], main_params);
    }
}

int main(int argc, char const *argv[])
{
	int 				server_sock_udp, error_code;
	struct addrinfo 	hints, *res_udp, *res_tcp;
	char				buffer[BUFFER_SIZE], *buffer_ptr, main_params[ASCII_LIMIT][BUFFER_SIZE];
	ssize_t 			n;
	user_t 				*user;
	struct sigaction    act;

	memset(&act, 0, sizeof act);

	act.sa_handler = SIG_IGN;

	if (sigaction(SIGPIPE, &act, NULL) == -1) {
		fprintf(stderr, "sigaction failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	parse_args (argc, (char** const) argv, main_params);

	/* Initialization of user structure */
	user = create_user();
	/*                                  */

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
	set_user_udp_addrinfo(user, res_udp);

	/* Socket UDP creation */
	server_sock_udp = socket(res_udp->ai_family, res_udp->ai_socktype, res_udp->ai_protocol);

	if (server_sock_udp == -1) {
		fprintf(stderr, "socket creation failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	set_user_server_sock_udp(user, server_sock_udp);
	/*			           */

	/* Initialization of hints structure for socket TCP */
	hints.ai_socktype = SOCK_STREAM;
	/*                                                  */
		
	error_code = getaddrinfo(main_params[PARAM_FSIP], main_params[PARAM_FSPORT], &hints, &res_tcp);

	if (error_code != 0) {
		fprintf(stderr, "getaddrinfo failed: %s\n", gai_strerror(error_code));
		exit(EXIT_FAILURE);
	}
	set_user_tcp_addrinfo(user, res_tcp);

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
		client_manager(user, buffer);
		/*                  */
	}
	return 0;
}