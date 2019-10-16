/* _______________________________________________________________________________
 * »»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
 * IST - RC 2019/2020
 *
 * answer-file-manager.c
 *
 * -------------------------------------------------------------------------------
 *
 * Made by: Bruno Meira (89421), Nikoletta Matsur (89513) and Vasco Faria (89559)
 *
 * «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««
 */

#include "answer-file-manager.h"

#include "file-manager.h"
#include "topic-file-manager.h"
#include "question-file-manager.h"
#include "../constants.h"

#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

int answer_exists(char *answer_path) {
	if (file_exists(answer_path)) {
		return ANSWER_ALREADY_EXISTS;
	}
	return ANSWER_DOESNT_EXIST;
}

int get_answers(char *topic_name, char *question_name, answer_t ***answers_list) {

	int  i, answers_number = 0, total_answers = 0;

	DIR    *d = NULL;
	FILE   *fp = NULL;
	struct dirent *dir = NULL;

	char id[USER_ID_SIZE + 1];
	char current_dir[MAX_FILENAME*9];
	char p[MAX_PATH] = TOPICS_PATH;
	char user_id_path[MAX_PATH], data_path[MAX_PATH];

	char *image, *image_ext, img_path[MAX_PATH];

	strcat(p, topic_name);
	if (topic_exists(p) == TOPIC_DOESNT_EXIST)
		return TOPIC_DOESNT_EXIST;

	strcat(p, QUESTIONS_PATH);
	strcat(p, question_name);
	if (question_exists(p) == QUESTION_DOESNT_EXIST)
		return QUESTION_DOESNT_EXIST;

	strcat(p, ANSWERS_PATH);

	total_answers = count_directories(p);

	d = opendir(p);

	answer_t **ans_list = (answer_t**) malloc(sizeof(answer_t*) * MAX_ANSWERS);

	for (i = 0; i < MAX_ANSWERS; i++) ans_list[i] = NULL;

	if (d) {
		i = 0;
		while ((dir = readdir(d)) != NULL) {
			if (dir->d_name[0] != '.') {

				strcpy(current_dir, p);
				strcat(current_dir, dir->d_name);
				strcat(current_dir, "/num.txt\0");
				fp = fopen(current_dir, "r");
				if (fp) {
					fscanf(fp, "%d", &answers_number);
					fclose(fp);
				}

				if (answers_number > total_answers - MAX_ANSWERS) {

					strcpy(current_dir, p);
					strcat(current_dir, dir->d_name);

					strcpy(user_id_path, current_dir);
					strcat(user_id_path, "/uid.txt\0");
					id[0] = '\0';
					fp = fopen(user_id_path, "r"); //TODO ERRO DO SISTEMA
					if (fp) {
						fscanf(fp, "%s", id);
						fclose(fp);
					}

					strcpy(data_path, current_dir);
					strcat(data_path, "/answer.txt\0");

					if (image = get_img_file(current_dir)) {
						strcpy(img_path, current_dir);
						strcat(img_path, "/\0");
						strcat(img_path, image);
						ans_list[i] = new_answer(dir->d_name, id, get_file_size(data_path, "r"), data_path, get_file_size(img_path, "r"), get_img_ext(image), img_path);
					} else {
						ans_list[i] = new_answer(dir->d_name, id, get_file_size(data_path, "r"), data_path, 0, NULL, NULL);
					}
				}
			}
		}
		// closedir(d);
	}

	*answers_list = ans_list;
	return SUCCESS;
}





int post_answer(char *topic_name, char *question_name, answer_t *answer) {

	FILE *fd;
	int error_code, actual_answers_number;
	char p[MAX_PATH] = TOPICS_PATH;
	char num_path[MAX_PATH] = "\0";
	char user_id_path[MAX_PATH] = "\0";
	char answer_data_path[MAX_PATH] = "\0";
	char answer_img_path[MAX_PATH] = "\0";
	strcat(p, topic_name);

	if (topic_exists(p) == TOPIC_DOESNT_EXIST)
		return TOPIC_DOESNT_EXIST;

	strcat(p, QUESTIONS_PATH);	
	strcat(p, question_name);

	if (question_exists(p) == QUESTION_DOESNT_EXIST)
		return QUESTION_DOESNT_EXIST;

	strcat(p, ANSWERS_PATH);

	if ((actual_answers_number = count_directories(p)) >= 99)
		return MAX_ANSWERS_REACHED;
	
	strcat(p, answer->title);

	if (answer_exists(p) == ANSWER_ALREADY_EXISTS)
		return ANSWER_ALREADY_EXISTS;

	strcpy(num_path, p);
	strcat(num_path, "/num.txt\0");

	strcpy(user_id_path, p);
	strcat(user_id_path, "/uid.txt\0");

	/* Create Answer directory */
	error_code = mkdir(p, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (error_code) {
		fprintf(stderr, "ERROR: Unable to create directory: %s: %s, %d\n", p, strerror(errno), errno);
		exit(error_code);
	}

	/* Create answer number file */
	fd = fopen(num_path, "ab+");
	fprintf(fd, "%d", actual_answers_number);
	fclose(fd);

	/* Create answer user_id file */
	fd = fopen(user_id_path, "ab+");
	fprintf(fd, "%s", answer->user_id);
	fclose(fd);

	if (answer->data_size != 0) {
		strcpy(answer_data_path, p);
		strcat(answer_data_path, "answer.txt\0");
		// write_file_data(answer_data_path, answer->data_size, answer->data);
	}

	if (answer->image_size != 0) {
		strcpy(answer_img_path, p);
		strcat(answer_img_path, "img.\0");
		strcat(answer_img_path, answer->image_ext);
		// write_file_data(answer_img_path, answer->image_size, answer->image);
	}

	return SUCCESS;
}