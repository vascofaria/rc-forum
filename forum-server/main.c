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

#include "protocol-manager/udp-manager.h"

#define MAX(A, B) (((A) >= (B)) ? (A) : (B))

#define PORT 		 "58000"
#define GROUP_NUMBER "1"
#define BUFFER_SIZE   128
#define MAX_CLIENTS   5
#define ASCII_LIMIT   256
#define MAX_ARGS_N    3
#define MAX_ARGS_L    128

enum param_types {
	PARAM_FSPORT  = (unsigned char)'p',
};

static void set_default_params(char main_params[ASCII_LIMIT][BUFFER_SIZE]) {
	strcpy(main_params[PARAM_FSPORT], PORT);
	strcat(main_params[PARAM_FSPORT], GROUP_NUMBER);
}

static void display_usage (const char* appName, char main_params[ASCII_LIMIT][BUFFER_SIZE]) {
    printf("Usage: %s [options]\n", appName);
    puts("\nOptions:                            (defaults)\n");
    printf("    p <INT>    [p]ort               (%s)\n", main_params['p']);
    exit(EXIT_FAILURE);
}

static void parse_args (int argc, char* const argv[], char main_params[ASCII_LIMIT][BUFFER_SIZE]) {
    int i;
    int opt;

    opterr = 0;

    set_default_params(main_params);

    while ((opt = getopt(argc, argv, "p:")) != -1) {
        switch (opt) {
            case 'p':
            	strcpy(main_params[PARAM_FSPORT], optarg);
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
	int              				server_sock_udp, server_sock_tcp, client_sock_tcp, error_code, max_fd;
	pid_t            			  	pid; 
	struct addrinfo 	   	 		*res_tcp, *res_udp, hints;
	char             				buffer[BUFFER_SIZE], *buffer_ptr, main_params[ASCII_LIMIT][BUFFER_SIZE];
	socklen_t 		 				addrlen_udp, addrlen_tcp;
	struct sockaddr_in      		addr_udp, addr_tcp;
	ssize_t                	 		n, n_write;
	fd_set 							read_fds;
	char *response;

	parse_args (argc, (char** const) argv, main_params);

	/* Initialization of hints structure for socket UDP */
	memset((void *) &hints, 0, sizeof(struct addrinfo));
	hints.ai_family   = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags    = AI_PASSIVE | AI_NUMERICSERV;
	/*									                */

	error_code = getaddrinfo(NULL, main_params[PARAM_FSPORT], &hints, &res_udp);

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
	
	/* Socket UDP bind */
	error_code = bind(server_sock_udp, res_udp->ai_addr, res_udp->ai_addrlen);

	if (error_code == -1) {
		fprintf(stderr, "socket bind failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	/*                 */

	/* Initialization of hints structure for socket TCP */
	hints.ai_socktype = SOCK_STREAM;
	/*                                                  */
		
	error_code = getaddrinfo(NULL, main_params[PARAM_FSPORT], &hints, &res_tcp);

	if (error_code != 0) {
		fprintf(stderr, "getaddrinfo failed: %s\n", gai_strerror(error_code));
		exit(EXIT_FAILURE);
	}

	/* Socket TCP creation */
	server_sock_tcp = socket(res_tcp->ai_family, res_tcp->ai_socktype, res_tcp->ai_protocol);

	if (server_sock_tcp == -1) {
		fprintf(stderr, "socket creation failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	/*			           */

	/* Socket TCP bind */
	error_code = bind(server_sock_tcp, res_tcp->ai_addr, res_tcp->ai_addrlen);

	if (error_code == -1) {
		fprintf(stderr, "socket bind failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	/*                 */

	/* Socket TCP listen */
	error_code = listen(server_sock_tcp, MAX_CLIENTS);

	if (error_code == -1) {
		fprintf(stderr, "socket listen failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	/*                   */

	max_fd = MAX(server_sock_udp, server_sock_tcp);

	while (1) {
		FD_ZERO(&read_fds);
		FD_SET(server_sock_tcp, &read_fds);
		FD_SET(server_sock_udp, &read_fds);

		do {
			error_code = select(max_fd + 1, &read_fds, (fd_set *) NULL, (fd_set *) NULL, (struct timeval *) NULL);
		}
		while (error_code == -1 && errno == EINTR);

		if (error_code == -1) {
			fprintf(stderr, "select failed: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}

		/* Comunication by TCP socket */
		if (FD_ISSET(server_sock_tcp, &read_fds)) {
			addrlen_tcp = sizeof(addr_tcp);

			do {
				client_sock_tcp = accept(server_sock_tcp, (struct sockaddr *) &addr_tcp, &addrlen_tcp);
			} 
			while (client_sock_tcp == -1 && errno == EINTR);

			if (client_sock_tcp == -1) {
				fprintf(stderr, "accept failed: %s\n", strerror(errno));
				exit(EXIT_FAILURE);
			}
			pid = fork();

			if (pid == -1) {
				fprintf(stderr, "fork failed: %s\n", strerror(errno));
				exit(EXIT_FAILURE);
			}

			else if (pid == 0) {
				error_code = close(server_sock_tcp);
				if (error_code == -1) {
					fprintf(stderr, "close failed: %s\n", strerror(errno));
					exit(EXIT_FAILURE);
				}

				error_code = close(server_sock_udp);
				if (error_code == -1) {
					fprintf(stderr, "close failed: %s\n", strerror(errno));
					exit(EXIT_FAILURE);
				}

				/* read from client */
				n = read(client_sock_tcp, buffer, BUFFER_SIZE);
				if (n == -1) {
					fprintf(stderr, "read failed: %s\n", strerror(errno));
					exit(EXIT_FAILURE);
				}
				/*                  */
				printf("%s %d\n", buffer, n);
				/**
				 * TODO: implement menu
				 */

				/* write on client */
				buffer_ptr = buffer;

				while (n > 0) {
					n_write = write(client_sock_tcp, buffer_ptr, n);
					if (n_write == -1) {
						fprintf(stderr, "write failed: %s\n", strerror(errno));
					}
					n          -= n_write;
					buffer_ptr += n_write;
				}
				/*                 */

				error_code = close(client_sock_tcp);
				if (error_code == -1) {
					fprintf(stderr, "close failed: %s\n", strerror(errno));
					exit(EXIT_FAILURE);
				}

				exit(EXIT_FAILURE);
			}

			else {
				do {
					error_code = close(client_sock_tcp);
				} 
				while (error_code == -1 && errno == EINTR);

				if (error_code == -1) {
					fprintf(stderr, "close failed: %s\n", strerror(errno));
					exit(EXIT_FAILURE);
				}
			}
		}

		/* Comunication by UDP socket */
		if (FD_ISSET(server_sock_udp, &read_fds)) {
			addrlen_udp = sizeof(addr_udp);

			/* read from clients */	
			n = recvfrom(server_sock_udp, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &addr_udp, &addrlen_udp);
			if (n == -1) {
				fprintf(stderr, "recvfrom failed: %s\n", strerror(errno));
				exit(EXIT_FAILURE);
			}
			buffer[n] = '\0';
			/*                   */

			response = udp_manager(buffer);
			printf("%s\n", response);

			/* write on client */
			n = sendto(server_sock_udp, buffer, n, 0, (struct sockaddr *) &addr_udp, addrlen_udp);
			if (n == -1) {
				fprintf(stderr, "sendto failed: %s\n", strerror(errno));
				exit(EXIT_FAILURE);
			}
			free(response);
			/*                 */
		}
		/*                            */
	}

	return 0;
}