/* _______________________________________________________________________________
 * »»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
 * IST - RC 2019/2020
 *
 * file-manager.c
 *
 * -------------------------------------------------------------------------------
 *
 * Made by: Bruno Meira (89421), Nikoletta Matsur (89513) and Vasco Faria (89559)
 *
 * «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««
 */

#include "file-manager.h"
#include "../entities/answer.h"
#include "../entities/question.h"
#include "../constants.h"

#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

int file_exists(char *file_path) {
	if (opendir(file_path)) {
		// closedir
		return TRUE;
	}
	// closedir();
	return FALSE;
}

char **list_directory(char* path) {

	int i = 0;
	char *dir_name;
	char current_dir[MAX_FILENAME*9];
	char id[USER_ID_SIZE + 1];
	DIR *d = NULL;
	FILE *fp = NULL;
	struct dirent *dir = NULL;
	d = opendir(path);

	char **dir_list = (char**) malloc(sizeof(char*) * MAX_TOPICS);

	for (i = 0; i < MAX_TOPICS; i++) dir_list[i] = NULL;

	if (d) {
		i = 0;
		while ((dir = readdir(d)) != NULL) {
			if (dir->d_name[0] != '.') {
				strcpy(id, "\0");
				strcpy(current_dir, path);
				dir_list[i] = (char*) malloc(sizeof(char) * (MAX_FILENAME + 1 + USER_ID_SIZE + 1));
				strcat(current_dir, dir->d_name);
				strcat(current_dir, "/uid.txt\0");
				fp = fopen(current_dir, "r"); //TODO ERRO DO SISTEMA
				if (fp) {
					fscanf(fp, "%s", id); 
					fclose(fp);
				}
				strcpy(dir_list[i], dir->d_name);
				strcat(dir_list[i], ":");
				strcat(dir_list[i++], id);
			}
		}
		// closedir(d);
	}
	return dir_list;
}

int get_file_size(const char *file_name, const char* mode) {
	FILE*    fp;
	long int size;

	if ((fp = fopen(file_name, mode)) != NULL) {
		
		fseek(fp, 0L, SEEK_END);
		size = ftell(fp);
		fclose(fp);

		return size;
	}
	return 0;
}

char* get_file_data(const char* file_name, const char* mode) {
	FILE*    fp;
	long int size;
	char*    data;

	if ((fp = fopen(file_name, mode)) != NULL) {
			
		fseek(fp, 0L, SEEK_END);
		size = ftell(fp);
		fseek(fp, 0L, SEEK_SET);

		data = (char*) malloc(sizeof(char) * (size + 1));

		fread(data, sizeof(char), size, fp);
		data[size + 1] = '\0';
		fclose(fp);
		return data;
	}
	return NULL;
}

void write_file_data(const char *file_path, const long int size, const char *data) {
	FILE *fp;
	int  n;
	
	if ((fp = fopen(file_path, "w")) != NULL) {
		n = fwrite(data, sizeof(char), size, fp);
		fclose(fp);
	}
}

char* get_img_ext(const char* path) {
	DIR *d = opendir(path);
	struct dirent *dir = NULL;
	char *ext;

	if (d) {
		while ((dir = readdir(d)) != NULL) {
			if (dir->d_name[0] == 'i') {
				ext = (char*) malloc(sizeof(char)*4);
				ext[0] = dir->d_name[4];
				ext[1] = dir->d_name[5];
				ext[2] = dir->d_name[6];
				ext[3] = '\0';
				return ext;
			}
		}
	}
	return NULL;
}

int count_directories(char *path) {
	int dirs_number = 0;
	DIR *d = NULL;
	struct dirent *dir = NULL;
	d = opendir(path);

	if (d) {
		while((dir = readdir(d)) != NULL) {
			if (dir->d_name[0] != '.') {
				dirs_number++;
			}
		}
	}
	return dirs_number;
}

int write_from_socket_to_file(int sock_tcp, char *file_path, int file_size) {
	int  i, n, file_fd, error_code;
	char c;

	file_fd = open(file_path, O_CREAT | O_WRONLY);

	if (file_fd == -1) {
		fprintf(stderr, "open failed: %s\n", strerror(errno));
		return FAILURE;
	}

	for (i = 0; i < file_size; i++) {
		do {
			n = read(sock_tcp, &c, 1);
			if (n == -1) {
				fprintf(stderr, "read from socket failed: %s\n", strerror(errno));
				return FAILURE;
			}
		} while (n == 0);

		do {
			n = write(file_fd, &c, 1);
			if (n == -1) {
				fprintf(stderr, "write to file failed: %s\n", strerror(errno));
				return FAILURE;
			}
		} while (n == 0);
	}

	error_code = close(file_fd);
	if (error_code == -1) {
		fprintf(stderr, "close failed: %s\n", strerror(errno));
		return FAILURE;
	}

	return SUCCESS;
}

int write_from_file_to_socket(int sock_tcp, char *file_path, int file_size) {
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
			if (n == -1) {
				fprintf(stderr, "read from file failed: %s\n", strerror(errno));
				return FAILURE;
			}
		} while (n == 0);

		do {
			n = write(sock_tcp, &c, 1);
			if (n == -1) {
				fprintf(stderr, "write to socket failed: %s\n", strerror(errno));
				return FAILURE;
			}
		} while (n == 0);
	}

	error_code = close(file_fd);
	if (error_code == -1) {
		fprintf(stderr, "close failed: %s\n", strerror(errno));
		return FAILURE;
	}

	return SUCCESS;
}
