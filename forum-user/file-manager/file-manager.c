#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "file-manager.h"
#include "../exceptions.h"

char*
get_file_size(const char *file_name, const char* mode) {
	FILE*    fp;
	long int size;
	char     *size_str;

	if (file_name != NULL) {
		if ((fp = fopen(file_name, mode)) != NULL) {
			
			fseek(fp, 0L, SEEK_END);
			size = ftell(fp);
			fclose(fp);

			size_str = (char*) malloc ((sizeof(size) / sizeof(long int)) + sizeof(char));

			if (size_str != NULL) {
				sprintf(size_str, "%ld", size);
				return size_str;
			}
		}
	} 
	return NULL;
}

char*
get_file_data(const char* file_name, const char* mode) {
	FILE*    fp;
	long int size;
	char*    data;

	if (file_name != NULL) {
		if ((fp = fopen(file_name, mode)) != NULL) {
			
			fseek(fp, 0L, SEEK_END);
			size = ftell(fp); 
			fseek(fp, 0L, SEEK_SET);

			data = (char*) malloc (sizeof(char) * (size + 1));

			if (data != NULL) {
				fread(data, sizeof(char), size, fp);
				data[size + 1] = '\0';
				fclose(fp);
				return data;
			}
		}
	}
	return NULL;
}	

char*
get_file_ext(const char* file_name) {
	char  *ext, *ext_aux;
	ext_aux = strchr(file_name, '.');
	ext     = (char *) malloc (sizeof(char) * (strlen(ext_aux) + 1));
	strcpy(ext, ext_aux);
	return ext;
}

void
write_to_tcp_socket(int sock_tcp, char *buffer) {
	int   n, nw;
	char *buffer_ptr;

	n          = strlen(buffer);
	buffer_ptr = buffer;

	while (n > 0) {
		nw = write(sock_tcp, buffer_ptr, n);
		if (nw == -1) {
			fprintf(stderr, "write failed: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		n -= nw;
		buffer_ptr += (sizeof(char) * nw);
	}
}

int 
read_from_tcp_socket(int sock_tcp, char *buffer, int size, char final_char) {
	int   n, nr;
	char *buffer_ptr;

	n = size;
	buffer_ptr = buffer;

	while (n > 0) {
		nr = read(sock_tcp, buffer_ptr, 1);
		if (nr == -1) {
			fprintf(stderr, "read failed: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}	
		if (*buffer_ptr == '\n' || *buffer_ptr == ' ' || *buffer_ptr == '\0') {
			if (*buffer_ptr != final_char) {
				return FAILURE;
			}
			buffer_ptr = '\0';
			return SUCCESS;
		}
		n--;
		buffer_ptr += (sizeof(char));
	}

	buffer[size] = '\0';

	return FAILURE;
}

void 
write_from_socket_to_file(int sock_tcp, char *file_path, int file_size) {
	int  i, n, file_fd, error_code;
	char c;

	file_fd = open(file_path, O_CREAT | O_WRONLY);

	if (file_fd == -1) {
		fprintf(stderr, "open failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < file_size; i++) {
		do {
			n = read(sock_tcp, &c, 1);
		} while (n == 0);

		do {
			n = write(file_fd, &c, 1);
		} while (n == 0);
	}

	error_code = close(file_fd);

	if (error_code == -1) {
		fprintf(stderr, "close failed: %s\n", strerror(errno));
	}
}

void 
write_from_file_to_socket(int sock_tcp, char *file_path, int file_size) {
	int  i, n, file_fd, error_code;
	char c;

	file_fd = open(file_path, O_CREAT | O_RDONLY);

	if (file_fd == -1) {
		fprintf(stderr, "open failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < file_size; i++) {
		do {
			n = read(file_fd, &c, 1);
		} while (n == 0);

		do {
			n = write(sock_tcp, &c, 1);
		} while (n == 0);
	}

	error_code = close(file_fd);

	if (error_code == -1) {
		fprintf(stderr, "close failed: %s\n", strerror(errno));
	}
}