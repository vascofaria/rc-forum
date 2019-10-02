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

#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

#define MAX_PATH      90
#define MAX_FILENAME  20
#define MAX_TOPICS    99
#define MAX_QUESTIONS 99

#define TOPICS_PATH    "./topics/\0"
#define QUESTIONS_PATH "/questions/\0"
#define ANSWERS_PATH   "/answers/\0"

int topic_exists(char *topic_path) {
	if (opendir(topic_path)) {
		return TOPIC_ALREADY_EXISTS;
	}
	return TOPIC_DOESNT_EXIST;
}

int question_exists(char *question_path) {
	if (opendir(question_path)) {
		return QUESTION_ALREADY_EXISTS;
	}
	return QUESTION_DOESNT_EXIST;
}

int answer_exists(char *answer_path) {
	if (opendir(answer_path)) {
		return ANSWER_ALREADY_EXISTS;
	}
	return ANSWER_DOESNT_EXIST;
}

char **list_directory(char* path) {

	int i = 0;
	char *dir_name;
	DIR *d = NULL;
	struct dirent *dir = NULL;
	d = opendir(path);

	char **dir_list = (char**) malloc(sizeof(char*) * MAX_TOPICS);

	if (d) {
		i = 0;
		while ((dir = readdir(d)) != NULL) {
			if (dir->d_name[0] != '.') {
				dir_list[i] = (char*) malloc(sizeof(char) * MAX_FILENAME);
				// printf("%s\n", dir->d_name);
				strcpy(dir_list[i++], dir->d_name);
				// TODO: Load user ID
			}
		}
		// closedir(d);
	}
	return dir_list;
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

int get_topics(char ***topics_list) {

	char p[MAX_PATH] = TOPICS_PATH;

	*topics_list = list_directory(p);

	// for (int i = 0; i<99 && (*topics_list)[i] != NULL; i++) {
	// 	printf("%s\n", (*topics_list)[i]);
	// }
	return SUCCESS;
}

int get_question(char *topic_name, char *question_name, question_t *question) {
	
	char p[MAX_PATH] = TOPICS_PATH;
	strcat(p, topic_name);

	if (topic_exists(p) == TOPIC_DOESNT_EXIST)
		return TOPIC_DOESNT_EXIST;

	strcat(p, QUESTIONS_PATH);
	strcat(p, question_name);

	if (question_exists(p) == QUESTION_DOESNT_EXIST)
		return QUESTION_DOESNT_EXIST;


	// TODO: load all the data to the question structure
}

int get_questions(char *topic_name, char ***questions_list) {

	char p[MAX_PATH] = TOPICS_PATH;
	strcat(p, topic_name);

	if (topic_exists(p) == TOPIC_DOESNT_EXIST)
		return TOPIC_DOESNT_EXIST;

	strcat(p, QUESTIONS_PATH);
	*questions_list = list_directory(p);

	// for (int i = 0; i<99 && (*questions_list)[i] != NULL; i++) {
	// 	printf("%s\n", (*questions_list)[i]);
	// }
	return SUCCESS;
}

int get_answers(char *topic_name, char *question_name, char ***answers_list) {

	char p[MAX_PATH] = TOPICS_PATH;
	strcat(p, topic_name);
	if (topic_exists(p) == TOPIC_DOESNT_EXIST)
		return TOPIC_DOESNT_EXIST;

	strcat(p, QUESTIONS_PATH);
	strcat(p, question_name);
	if (question_exists(p) == QUESTION_DOESNT_EXIST)
		return QUESTION_DOESNT_EXIST;

	strcat(p, ANSWERS_PATH);
	*answers_list = list_directory(p);
	return SUCCESS;
}

int post_topic(char *topic_name, char *user_id) {

	int error_code;
	char p[MAX_PATH] = TOPICS_PATH;
	char user_id_path[MAX_PATH] = "\0";
	strcat(p, topic_name);

	FILE *fd;
	strcat(user_id_path, p);
	strcat(user_id_path, "/\0");
	strcat(user_id_path, topic_name);
	strcat(user_id_path, "_uid.txt\0");

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

int post_question(char *topic_name, question_t *question, char *user_id) {

	FILE *fd;
	int error_code;
	char p[MAX_PATH] = TOPICS_PATH;
	char user_id_path[MAX_PATH] = "\0";
	strcat(p, topic_name);

	if (topic_exists(p) == TOPIC_DOESNT_EXIST)
		return TOPIC_DOESNT_EXIST;

	strcat(p, QUESTIONS_PATH);

	if (count_directories(p) >= 99)
		return MAX_QUESTIONS_REACHED;
	
	strcat(p, question->title);

	if (question_exists(p) == QUESTION_ALREADY_EXISTS)
		return QUESTION_ALREADY_EXISTS;

	strcat(user_id_path, p);
	strcat(user_id_path, "/\0");
	// strcat(user_id_path, question->title);
	strcat(user_id_path, "uid.txt\0");

	/* Create Question directory */
	error_code = mkdir(p, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (error_code) {
		fprintf(stderr, "ERROR: Unable to create directory: %s: %s, %d\n", p, strerror(errno), errno);
		exit(error_code);
	}

	/* Create question user_id file */
	fd = fopen(user_id_path, "ab+");
	fprintf(fd, "%s", user_id);
	fclose(fd);

	/* Create question answers directory */
	strcat(p, ANSWERS_PATH);
	error_code = mkdir(p, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (error_code) {
		fprintf(stderr, "ERROR: Unable to create directory: %s: %s\n", p, strerror(errno));
		exit(error_code);
	}

	if (question->data_size != 0) {
		// strcat(p, "/question.txt");
		// TODO: question->data => question.txt
	}

	if (question->image_size != 0) {
		// strcat(p, "/question.ext");
		// TODO: question->image => question.ext
	}

	return SUCCESS;
}

int post_answer(char *topic_name, char *question_name, answer_t *answer, char *user_id) {

	FILE *fd;
	int error_code;
	char p[MAX_PATH] = TOPICS_PATH;
	char user_id_path[MAX_PATH] = "\0";
	strcat(p, topic_name);

	if (topic_exists(p) == TOPIC_DOESNT_EXIST)
		return TOPIC_DOESNT_EXIST;

	strcat(p, QUESTIONS_PATH);	
	strcat(p, question_name);

	if (question_exists(p) == QUESTION_DOESNT_EXIST)
		return QUESTION_DOESNT_EXIST;

	strcat(p, ANSWERS_PATH);

	if (count_directories(p) >= 99)
		return MAX_ANSWERS_REACHED;
	
	strcat(p, answer->title);

	if (answer_exists(p) == ANSWER_ALREADY_EXISTS)
		return ANSWER_ALREADY_EXISTS;

	strcat(user_id_path, p);
	strcat(user_id_path, "/\0");
	// TODO: USERID

	/* Create Answer directory */
	error_code = mkdir(p, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (error_code) {
		fprintf(stderr, "ERROR: Unable to create directory: %s: %s, %d\n", p, strerror(errno), errno);
		exit(error_code);
	}

	/* Create answer user_id file */
	fd = fopen(user_id_path, "ab+");
	fprintf(fd, "%s", user_id);
	fclose(fd);

	if (answer->data_size != 0) {
		// strcat(p, "/answer.txt");
		// TODO: answer->data => answer.txt
	}

	if (answer->image_size != 0) {
		// strcat(p, "/answer.ext");
		// TODO: answer->image => answer.ext
	}

	return SUCCESS;
}
