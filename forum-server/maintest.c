#include <stdio.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define HOST 		 "xsy"
#define SERV 		 "58000"
#define BUFFER_SIZE  128

int main(int argc, char const *argv[])
{
	int              		sock_udp, sock_tcp, error_code;
	struct addrinfo 	   *res, hints;
	char             		buffer[BUFFER_SIZE], buffer_ptr;
	socklen_t 		 		addrlen_udp, addrlen_tcp;
	struct sockaddr_in      addr_udp, addr_tcp;
	ssize_t                 n;

	/* Initialization of hints structure for socket UDP */
	memset((void *) &hints, 0, sizeof(struct addrinfo));
	hints.ai_family   = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags    = AI_NUMERICSERV;
	/*									                */

	error_code = getaddrinfo(HOST, SERV, &hints, &res);

	if (error_code != 0) {
		fprintf(stderr, "getaddrinfo failed: %s\n", gai_strerror(error_code));
		exit(EXIT_FAILURE);
	}

	/* Socket UDP creation */
	sock_udp = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	if (sock_udp == -1) {
		fprintf(stderr, "socket creation failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	/*			           */

	while (1) {
		addrlen_udp = sizeof(addr_udp);
		n = read(0, buffer, BUFFER_SIZE);
		n = sendto(sock_udp, buffer, n, 0, res->ai_addr, res->ai_addrlen);
		n = recvfrom(sock_udp, buffer, n, 0, (struct sockaddr *) &addr_udp, &addrlen_udp);
		write(1, buffer, n);
	}
	return 0;	
}