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

#define MAX(A, B) (((A) >= (B)) ? (A) : (B))

#define SERV 		 "58000"
#define BUFFER_SIZE  128
#define MAX_CLIENTS  5
#define MAX_ARGS_N	 3
#define MAX_ARGS_L   20

int main(int argc, char const *argv[])
{
	int              		server_sock_udp, server_sock_tcp, client_sock_tcp, error_code, max_fd;
	pid_t            		pid; 
	struct addrinfo 	   *res_tcp, *res_udp, hints;
	char             		buffer[BUFFER_SIZE], *buffer_ptr;
	socklen_t 		 		addrlen_udp, addrlen_tcp;
	struct sockaddr_in      addr_udp, addr_tcp;
	ssize_t                 n, n_write;
	fd_set 					read_fds;


	/* Initialization of hints structure for socket UDP */
	memset((void *) &hints, 0, sizeof(struct addrinfo));
	hints.ai_family   = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags    = AI_PASSIVE | AI_NUMERICSERV;
	/*									                */

	error_code = getaddrinfo(NULL, SERV, &hints, &res_udp);

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
		
	error_code = getaddrinfo(NULL, SERV, &hints, &res_tcp);

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

		error_code = select(max_fd + 1, &read_fds, (fd_set *) NULL, (fd_set *) NULL, (struct timeval *) NULL);

		if (error_code == -1) {
			fprintf(stderr, "select failed: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}

		/* Comunication by TCP socket */
		if (FD_ISSET(server_sock_tcp, &read_fds)) {
			addrlen_tcp = sizeof(addr_tcp);

			client_sock_tcp = accept(server_sock_tcp, (struct sockaddr *) &addr_tcp, &addrlen_tcp);

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
				close(server_sock_tcp);
				close(server_sock_udp);

				/* read from client */
				n = read(client_sock_tcp, buffer, BUFFER_SIZE);
				if (n == -1) {
					fprintf(stderr, "read failed: %s\n", strerror(errno));
					exit(EXIT_FAILURE);
				}
				/*                  */

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
			}

			else {

			}
		}
		/*                          */

		/* Comunication by UDP socket */
		if (FD_ISSET(server_sock_udp, &read_fds)) {
			/* read from clients */
			n = recvfrom(server_sock_udp, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &addr_udp, &addrlen_udp);
			if (n == -1) {
				fprintf(stderr, "recvfrom failed: %s\n", strerror(errno));
				exit(EXIT_FAILURE);
			}
			/*                   */
			write(1, "received: ", 10);
			write(1, buffer, n);
			write(1, "ola\n", 4);
			/* write on client */
			n = sendto(server_sock_udp, buffer, n, 0, (struct sockaddr *) &addr_udp, addrlen_udp);
			if (n == -1) {
				fprintf(stderr, "sendto failed: %s\n", strerror(errno));
				exit(EXIT_FAILURE);
			}
			/*                 */
		}
		/*                            */
	}

	return 0;
}