/* _______________________________________________________________________________
 * »»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
 * IST - RC 2019/2020
 *
 * question-file-manager.c
 *
 * -------------------------------------------------------------------------------
 *
 * Made by: Bruno Meira (89421), Nikoletta Matsur (89513) and Vasco Faria (89559)
 *
 * «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««
 */

#include "question-file-manager.h"

#include "file-manager.h"
#include "topic-file-manager.h"
#include "answer-file-manager.h"
#include "../constants.h"

#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

int question_exists(char *question_path) {
	if (file_exists(question_path)) {
		return QUESTION_ALREADY_EXISTS;
	}
	return QUESTION_DOESNT_EXIST;
}

int get_question(char *topic_name, char *question_title, question_t **question) {
	
	int error_code;
	char p[MAX_PATH] = TOPICS_PATH, user_id_path[MAX_PATH], question_user_id[USER_ID_SIZE+1];
	char question_data_path[MAX_PATH], question_image_path[MAX_PATH];
	char *question_img_ext = NULL;
	strcat(p, topic_name);
	FILE *fp = NULL;

	answer_t **answers_list;

	if (topic_exists(p) == TOPIC_DOESNT_EXIST)
		return TOPIC_DOESNT_EXIST;

	strcat(p, QUESTIONS_PATH);
	strcat(p, question_title);

	if (question_exists(p) == QUESTION_DOESNT_EXIST)
		return QUESTION_DOESNT_EXIST;

	strcpy(user_id_path, p);
	strcat(user_id_path, "/uid.txt\0");
	fp = fopen(user_id_path, "r");
	if (fp) {
		fscanf(fp, "%s", question_user_id); 
		fclose(fp);
	}

	strcpy(question_data_path, p);
	strcat(question_data_path, "/question.txt\0");


	error_code = get_answers(topic_name, question_title, &answers_list);
	if (error_code) {
		return error_code;
	}

	question_img_ext = get_img_ext(p);

	// TODO: load all the data to the question structure
	if (question_img_ext) {

		strcpy(question_image_path, p);
		strcat(question_image_path, "/img.\0");
		strcat(question_image_path, question_img_ext);

		*question = new_question(question_title, question_user_id, get_file_size(question_data_path, "r"), question_data_path, get_file_size(question_image_path, "rb"), question_img_ext, question_image_path, answers_list);
		// frees
		free(question_img_ext);
	} else {
		*question = new_question(question_title, question_user_id, get_file_size(question_data_path, "r"), question_data_path, 0, NULL, NULL, answers_list);
		// frees
	}

	return SUCCESS;
}

int get_questions(char *topic_name, char ***questions_list) {
	int i, answers_number;
	DIR *d = NULL;
	FILE *fp = NULL;
	struct dirent *dir = NULL;
	char id[USER_ID_SIZE + 1];
	char answers_number_str[3] = "\0";
	char current_dir[MAX_FILENAME*9];
	char p[MAX_PATH] = TOPICS_PATH;
	strcat(p, topic_name);

	if (topic_exists(p) == TOPIC_DOESNT_EXIST)
		return TOPIC_DOESNT_EXIST;

	strcat(p, QUESTIONS_PATH);

	d = opendir(p);

	char **dir_list = (char**) malloc(sizeof(char*) * MAX_QUESTIONS);

	for (i = 0; i < MAX_QUESTIONS; i++) dir_list[i] = NULL;

	if (d) {
		i = 0;
		while ((dir = readdir(d)) != NULL) {

			if (dir->d_name[0] != '.') {
				dir_list[i] = (char*) malloc(sizeof(char) * (MAX_FILENAME + 1 + USER_ID_SIZE + 1 + 2 + 1));

				strcpy(current_dir, p);
				strcat(current_dir, dir->d_name);
				strcat(current_dir, "/uid.txt\0");
				strcpy(id, "\0");
				fp = fopen(current_dir, "r"); //TODO ERRO DO SISTEMA
				if (fp) {
					fscanf(fp, "%s", id);
					fclose(fp);
				}

				strcpy(current_dir, p);
				strcat(current_dir, dir->d_name);
				strcat(current_dir, ANSWERS_PATH);
				answers_number = count_directories(current_dir);

				if (answers_number < 10) {
					answers_number_str[0] = '0';
					answers_number_str[1] = '0' + answers_number;
				} else {
					answers_number_str[0] = '1';
					answers_number_str[1] = '0';
				}
				answers_number_str[2] = '\0';

				strcpy(dir_list[i], dir->d_name);
				strcat(dir_list[i], ":");
				strcat(dir_list[i], id);
				strcat(dir_list[i], ":");
				strcat(dir_list[i++], answers_number_str);
			}
		}
	}

	*questions_list = dir_list;
	return SUCCESS;
}

int post_question(char *topic_name, question_t *question) {

	FILE *fd;
	int  error_code;
	char p[MAX_PATH] = TOPICS_PATH;
	char question_data_path[MAX_PATH] = "\0";
	char question_img_path[MAX_PATH] = "\0";
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
	strcat(user_id_path, "/uid.txt\0");

	/* Create Question directory */
	error_code = mkdir(p, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (error_code) {
		fprintf(stderr, "ERROR: Unable to create directory: %s: %s, %d\n", p, strerror(errno), errno);
		exit(error_code);
	}

	/* Create question user_id file */
	fd = fopen(user_id_path, "w");
	if (fd == NULL) {
		fprintf(stderr, "ERROR: coudnt open file: %s: %s, %d\n", user_id_path, strerror(errno), errno);
		return FAILURE;
	}
	fprintf(fd, "%s", question->user_id);
	fclose(fd);

	/* Create question.txt file */
	if (question->data_size != 0) {
		strcpy(question_data_path, p);
		strcat(question_data_path, "/question.txt\0");
		printf("%s - %s\n", question->data_path, question_data_path);
		error_code = move_file(question->data_path, question_data_path);
		if (error_code) {
			return FAILURE;
		}
	}

	/* Create question image file */
	if (question->image_size != 0) {
		strcpy(question_img_path, p);
		strcat(question_img_path, "/img.\0");
		strcat(question_img_path, question->image_ext);
		printf("%s - %s\n", question_img_path, question->image_path);

		error_code = move_file(question->image_path, question_img_path);
		if (error_code) {
			return FAILURE;
		}
	}

	/* Create question answers directory */
	strcat(p, ANSWERS_PATH);
	error_code = mkdir(p, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (error_code) {
		fprintf(stderr, "ERROR: Unable to create directory: %s: %s\n", p, strerror(errno));
		return FAILURE;
	}

	return SUCCESS;
}
