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
	strcpy(ext, ext_aux + sizeof(char));
	return ext;
}

int 
write_to_tcp_socket(int socket_tcp, char *buffer, char final_char) {
    int len, n;
    char *buffer_ptr = buffer;

    len = strlen(buffer);

    while(len > 0) {
        n = write(socket_tcp, buffer_ptr, len);
        if (n == -1) {
            fprintf(stderr, "write to socket failed: %s\n", strerror(errno));
            return FAILURE;
        }
        len -= n;
        buffer_ptr += (sizeof(char) * n);
    }

    if (final_char != '\0') {
        do {
            n = write(socket_tcp, &final_char, 1);
            if (n == -1) {
                fprintf(stderr, "write to socket failed: %s\n", strerror(errno));
                return FAILURE;
            }
        } while(n == 0);
    }

    return SUCCESS;
}

int 
read_from_tcp_socket(int socket_tcp, char *buffer, int size, char final_char) {
	int len, n;
	char *buffer_ptr = buffer;
	char space[2];

	len = size;

	while (len > 0) {
		n = read(socket_tcp, buffer_ptr, 1);
		if (n == -1) {
			fprintf(stderr, "read from socket failed: %s\n", strerror(errno));
			return FAILURE;
		}
		if (*buffer_ptr == ' ' || *buffer_ptr == '\n' || *buffer_ptr == '\0') {
			if (*buffer_ptr != final_char) {
				return FAILURE;
			}
			*buffer_ptr = '\0';
			return SUCCESS;
		}
		len--;
		buffer_ptr += (sizeof(char));
	}

	if (size == 0) {
		do {
			n = read(socket_tcp, space, 1);
			if (n == -1) {
				fprintf(stderr, "read from socket failed: %s\n", strerror(errno));
				return FAILURE;
			}
		}	while (n == 0);
		if (space[0] == final_char) {
			return SUCCESS;
		}
	} else if (final_char == '\0') {
		return SUCCESS;
	}
	return FAILURE;
}

void 
write_from_socket_to_file(int sock_tcp, char *file_path, int file_size) {
	int  i, n, file_fd, error_code;
	unsigned char c;

	file_fd = open(file_path, O_CREAT | O_WRONLY, 0644);

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

int 
write_from_file_to_socket(int sock_tcp, char *file_path, int file_size) {
    int  i, n, file_fd, error_code;
    unsigned char c;

    file_fd = open(file_path, O_CREAT | O_RDONLY);

    if (file_fd == -1) {
        fprintf(stderr, "open failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < file_size; i++) {
        do {
            n = read(file_fd, &c, 1);
        } while (n == -1 && errno == EINTR);

        do {
            n = write(sock_tcp, &c, 1);
        } while (n == -1 && errno == EINTR);
    }

    error_code = close(file_fd);
    if (error_code == -1) {
        fprintf(stderr, "close failed: %s\n", strerror(errno));
        return FAILURE;
    }

    return SUCCESS;
}