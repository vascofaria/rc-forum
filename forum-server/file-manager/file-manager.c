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

char **list_directory(char* path) {

	int i = 0;
	char *dir_name;
	char current_dir[MAX_FILENAME*2]; 
	char id[USER_ID_SIZE + 10];
	DIR *d = NULL;
	FILE *fp = NULL;
	struct dirent *dir = NULL;
	d = opendir(path);

	char **dir_list = (char**) malloc(sizeof(char*) * MAX_TOPICS);

	if (d) {
		i = 0;
		while ((dir = readdir(d)) != NULL) {
			if (dir->d_name[0] != '.') {
				strcpy(id, "\0");
				strcpy(current_dir, TOPICS_PATH); 
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

int get_topics(char ***topics_list) {

	char p[MAX_PATH] = TOPICS_PATH;

	*topics_list = list_directory(p);

	// for (int i = 0; i<99 && (*topics_list)[i] != NULL; i++) {
	// 	printf("%s\n", (*topics_list)[i]);
	// }
	return SUCCESS;
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
	//strcat(user_id_path, topic_name);
	strcat(user_id_path, "uid.txt\0");

	if (topic_exists(p) == TOPIC_ALREADY_EXISTS)
		return TOPIC_ALREADY_EXISTS;

	/* Create Topic directory */
	error_code = mkdir(p, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (error_code) {
		fprintf(stderr, "ERROR: Unable to create directory: %s: %s, %d\n", p, strerror(errno), errno);
		// exit(error_code);
		return FAILURE;
	} //VASCO- AQUI DEVIA DEVOLVER SE ALGO P usecases escreverem NOK 

	/* Create topic user_id file */
	fd = fopen(user_id_path, "ab+");
	fprintf(fd, "%s", user_id);
	fclose(fd);

	//VASCO - falta dares erro FUL

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

int post_question(char *topic_name, char *question_name, char *user_id) {

	FILE *fd;
	int error_code;
	char p[MAX_PATH] = TOPICS_PATH;
	char user_id_path[MAX_PATH] = "\0";
	strcat(p, topic_name);

	if (topic_exists(p) == TOPIC_DOESNT_EXIST)
		return TOPIC_DOESNT_EXIST;

	strcat(p, "/\0");
	strcat(p, QUESTIONS_PATH);
	strcat(p, question_name);

	if (question_exists(p) == QUESTION_ALREADY_EXISTS)
		return QUESTION_ALREADY_EXISTS;

	strcat(user_id_path, p);
	strcat(user_id_path, "/\0");
	strcat(user_id_path, question_name);
	strcat(user_id_path, "_uid.txt\0");

	/* Create Question directory */
	error_code = mkdir(p, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (error_code) {
		fprintf(stderr, "ERROR: Unable to create directory: %s: %s, %d\n", p, strerror(errno), errno);
		exit(error_code);
	}

	/* Create topic user_id file */
	fd = fopen(user_id_path, "ab+");
	fprintf(fd, "%s", user_id);
	fclose(fd);

	/* Create topic questions directory */
	strcat(p, ANSWERS_PATH);
	error_code = mkdir(p, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (error_code) {
		fprintf(stderr, "ERROR: Unable to create directory: %s: %s\n", p, strerror(errno));
		exit(error_code);
	}
	return SUCCESS;
}

/*------------------------------------------------------*/
/*------------------------TESTING-----------------------*/
/*      gcc file-manager.c file-manager.h -o run        */
/*------------------------------------------------------*/

// int main(int argc, char const *argv[]) {
// 	
// 	int i, error_code = 0;
// 	char topic_name[MAX_FILENAME] = "Topic07\0";
// 	char question_name[MAX_FILENAME] = "Question02\0";
// 	char **topics_list = NULL;
// 	char **questions_list = NULL;
// 	char **answers_list = NULL;
// 
// 	/* TEST POST TOPIC */
// 	// error_code = post_topic(topic_name, "89559\0");
// 	// if (error_code == TOPIC_ALREADY_EXISTS) {
// 	// 	fprintf(stderr, "Topic %s already exists\n", topic_name);
// 	// 	exit(TOPIC_ALREADY_EXISTS);
// 	// }
// 
// 	/* TEST POST QUESTION */
// 	// error_code = post_question(topic_name, question_name, "89559\0");
// 	// if (error_code == TOPIC_DOESNT_EXIST) {
// 	// 	fprintf(stderr, "Topic %s doesnt exist\n", topic_name);
// 	// 	exit(TOPIC_DOESNT_EXIST);
// 	// } else if (error_code == QUESTION_ALREADY_EXISTS) {
// 	// 	fprintf(stderr, "Question %s already exists\n", question_name);
// 	// 	exit(QUESTION_ALREADY_EXISTS);
// 	// }
// 
// 	/* TEST GET TOPICS */
// 	error_code = get_topics(&topics_list);
// 	int j;
// 	for(j=0; j < MAX_TOPICS; j++) {
// 		printf("%s", topics_list[j]);
// 		printf("\n");
// 	}
// 
// 	
// 
// 	/* TEST GET QUESTIONS */
// 	// error_code = get_questions(topic_name, &questions_list);
// 	// if (error_code == TOPIC_DOESNT_EXIST) {
// 	// 	fprintf(stderr, "ERROR: TOPIC_DOESNT_EXIST\n");
// 	// 	exit(TOPIC_DOESNT_EXIST);
// 	// }
// 
// 	// error_code = get_answers(topic_name, question_name, &answers_list);
// 	// if (error_code == TOPIC_DOESNT_EXIST) {
// 	// 	fprintf(stderr, "ERROR: TOPIC_DOESNT_EXIST\n");
// 	// 	exit(TOPIC_DOESNT_EXIST);
// 	// } else if (error_code == QUESTION_DOESNT_EXIST) {
// 	// 	fprintf(stderr, "ERROR: QUESTION_DOESNT_EXIST\n");
// 	// 	exit(QUESTION_DOESNT_EXIST);
// 	// }
// 
// 	// for (i = 0; i< MAX_TOPICS && topics_list[i] != NULL; i++)
// 	// 	printf("%s\n", topics_list[i]);
// 
// 	// for (i=0; i < MAX_TOPICS && questions_list[i] != NULL; i++)
// 	// 	printf("%s\n", questions_list[i]);
// 
// 	// for (i=0; i < MAX_TOPICS && answers_list[i] != NULL; i++)
// 	// 	printf("%s\n", answers_list[i]);
// 
// 	free_list(topics_list);
// 	free_list(questions_list);
// 	free_list(answers_list);
// 
// 	return 0;
// }
