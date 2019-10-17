/* _______________________________________________________________________________
 * »»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
 * IST - RC 2019/2020
 *
 * topic-file-manager.c
 *
 * -------------------------------------------------------------------------------
 *
 * Made by: Bruno Meira (89421), Nikoletta Matsur (89513) and Vasco Faria (89559)
 *
 * «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««
 */

#include "topic-file-manager.h"

#include "file-manager.h"
#include "../constants.h"

#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

int topic_exists(char *topic_path) {
	if (file_exists(topic_path)) {
		return TOPIC_ALREADY_EXISTS;
	}
	return TOPIC_DOESNT_EXIST;
}

int get_topics(char ***topics_list) {

	char path[MAX_PATH] = TOPICS_PATH;

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
	*topics_list = dir_list;

	return SUCCESS;
}

int post_topic(char *user_id, char *topic_name) {

	int error_code;
	char p[MAX_PATH] = TOPICS_PATH;
	char user_id_path[MAX_PATH] = "\0";
	strcat(p, topic_name);

	FILE *fd;
	strcat(user_id_path, p);
	strcat(user_id_path, "/\0");
	//strcat(user_id_path, topic_name);
	strcat(user_id_path, "uid.txt\0");

	if (topic_exists(p) == TOPIC_ALREADY_EXISTS)
		return TOPIC_ALREADY_EXISTS;

	if (count_directories(TOPICS_PATH) >= 99) {
		return MAX_TOPICS_REACHED;
	}

	/* Create Topic directory */
	error_code = mkdir(p, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (error_code) {
		fprintf(stderr, "ERROR: Unable to create directory: %s: %s, %d\n", p, strerror(errno), errno);
		// exit(error_code);
		return FAILURE;
	}

	/* Create topic user_id file */
	fd = fopen(user_id_path, "ab+");
	fprintf(fd, "%s", user_id);
	fclose(fd);
	
	/* Create topic questions directory */
	strcat(p, QUESTIONS_PATH);
	error_code = mkdir(p, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (error_code) {
		fprintf(stderr, "ERROR: Unable to create directory: %s: %s\n", p, strerror(errno));
		// exit(error_code);
		return FAILURE;
	}
	return SUCCESS;
}