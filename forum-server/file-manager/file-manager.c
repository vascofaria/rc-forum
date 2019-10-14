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

static char **list_directory(char* path) {

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

static int get_file_size(const char *file_name, const char* mode) {
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

static char* get_file_data(const char* file_name, const char* mode) {
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

static void write_file_data(const char *file_path, const long int size, const char *data) {
	FILE *fp;
	int  n;
	
	if ((fp = fopen(file_path, "w")) != NULL) {
		n = fwrite(data, sizeof(char), size, fp);
		fclose(fp);
	}
}

static char* get_img_ext(const char* path) {
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

int get_topics(char ***topics_list) {

	char p[MAX_PATH] = TOPICS_PATH;

	*topics_list = list_directory(p);

	// for (int i = 0; i<99 && (*topics_list)[i] != NULL; i++) {
	// 	printf("%s\n", (*topics_list)[i]);
	// }
	return SUCCESS;
}

int get_question(char *topic_name, char *question_title, question_t **question) {
	
	char p[MAX_PATH] = TOPICS_PATH, user_id_path[MAX_PATH], question_user_id[USER_ID_SIZE+1];
	char question_data_path[MAX_PATH], question_image_path[MAX_PATH];
	char *question_img_ext = NULL;
	strcat(p, topic_name);
	FILE *fp = NULL;

	if (topic_exists(p) == TOPIC_DOESNT_EXIST)
		return TOPIC_DOESNT_EXIST;

	strcat(p, QUESTIONS_PATH);
	strcat(p, question_title);

	if (question_exists(p) == QUESTION_DOESNT_EXIST)
		return QUESTION_DOESNT_EXIST;

	strcpy(user_id_path, p);
	strcat(user_id_path, "/uid.txt\0");
	fp = fopen(p, "r");
	if (fp) {
		fscanf(fp, "%s", question_user_id); 
		fclose(fp);
	}

	strcpy(question_data_path, p);
	strcat(question_data_path, "/question.txt\0");

	question_img_ext = get_img_ext(p);

	// TODO: load all the data to the question structure
	if (question_img_ext) {

		strcpy(question_image_path, p);
		strcat(question_image_path, "/img.\0");
		strcat(question_image_path, question_img_ext);

		printf("%s\n", question_image_path);

		*question = new_question(question_title, question_user_id, get_file_size(question_data_path, "r"), get_file_data(question_data_path, "r"), get_file_size(question_image_path, "rb"), question_img_ext, get_file_data(question_image_path, "rb"), NULL);
		// frees
		free(question_img_ext);
	} else {
		*question = new_question(question_title, question_user_id, get_file_size(question_data_path, "r"), get_file_data(question_data_path, "r"), 0, NULL, NULL, NULL);
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

int get_answers(char *topic_name, char *question_name, char ***answers_list) {
	int i, answers_number = 0, total_answers = 0;
	DIR *d = NULL;
	FILE *fp = NULL;
	struct dirent *dir = NULL;
	char id[USER_ID_SIZE + 1];
	char current_dir[MAX_FILENAME*9];
	char p[MAX_PATH] = TOPICS_PATH;
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

	char **dir_list = (char**) malloc(sizeof(char*) * MAX_ANSWERS);

	for (i = 0; i < MAX_ANSWERS; i++) dir_list[i] = NULL;

	if (d) {
		i = 0;
		while ((dir = readdir(d)) != NULL) {
			if (dir->d_name[0] != '.') {

				strcpy(current_dir, p);
				strcat(current_dir, "num.txt\0");
				fp = fopen(current_dir, "r");
				if (fp) {
					fscanf(fp, "%d", &answers_number);
					fclose(fp);
				}

				if (answers_number > total_answers - MAX_ANSWERS) {

					dir_list[i] = (char*) malloc(sizeof(char) * (MAX_FILENAME + 1 + USER_ID_SIZE + 1));

					strcpy(current_dir, p);
					strcat(current_dir, dir->d_name);
					strcat(current_dir, "/uid.txt\0");
					strcpy(id, "\0");
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
		}
		// closedir(d);
	}

	*answers_list = dir_list;
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
	strcat(user_id_path, "/\0");
	strcat(user_id_path, "uid.txt\0");

	/* Create Question directory */
	error_code = mkdir(p, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (error_code) {
		fprintf(stderr, "ERROR: Unable to create directory: %s: %s, %d\n", p, strerror(errno), errno);
		exit(error_code);
	}

	/* Create question user_id file */
	fd = fopen(user_id_path, "ab+");
	fprintf(fd, "%s", question->user_id);
	fclose(fd);

	/* Create question.txt file */
	if (question->data_size != 0) {
		strcpy(question_data_path, p);
		strcat(question_data_path, "/question.txt\0");
		write_file_data(question_data_path, question->data_size, question->data);
	}

	/* Create question image file */
	if (question->image_size != 0) {
		strcpy(question_img_path, p);
		strcat(question_img_path, "/img.\0");
		strcat(question_img_path, question->image_ext);
		write_file_data(question_img_path, question->image_size, question->image);
	}

	/* Create question answers directory */
	strcat(p, ANSWERS_PATH);
	error_code = mkdir(p, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (error_code) {
		fprintf(stderr, "ERROR: Unable to create directory: %s: %s\n", p, strerror(errno));
		exit(error_code);
	}

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
		write_file_data(answer_data_path, answer->data_size, answer->data);
	}

	if (answer->image_size != 0) {
		strcpy(answer_img_path, p);
		strcat(answer_img_path, "img.\0");
		strcat(answer_img_path, answer->image_ext);
		write_file_data(answer_img_path, answer->image_size, answer->image);
	}

	return SUCCESS;
}
