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
// #include "question.h"

#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdio.h>

#define MAX_FILENAME 10
#define MAX_QUESTIONS 10
#define MAX_TOPICS 50
#define MAX_PATH 20


char **list_directory(char* path) {
	DIR *d;
	struct dirent *dir;
	d = opendir(path);

	int i = 0;
	char *dir_name;

	char **dir_list = (char**) malloc(sizeof(char*) * MAX_TOPICS);

	if (d) {
		while ((dir = readdir(d)) != NULL) {
			if (dir->d_name[0] != '.') {
				printf("%s:%d\n", dir->d_name, dir->d_ino);
				dir_name = (char*) malloc(sizeof(char) * MAX_FILENAME);
				strcpy(dir_name, dir->d_name);
				dir_list[i++] = dir_name;
			}
		}
		closedir(d);
	}

	return dir_list;
}

char **get_topics() {
	char p[MAX_PATH] = "../files\0";
	return list_directory(p);
}

char **get_questions(char *topic_name) {
	char p[MAX_PATH] = "../files/Topic01\0";
	return list_directory(p);
}

/*------------------------------------------------------*/
/*------------------------TESTING-----------------------*/
/*      gcc file-manager.c file-manager.h -o run        */
/*------------------------------------------------------*/

int main(int argc, char const *argv[]) {
	
	char **buffer = get_questions(NULL);

	for (int i=0; i<MAX_TOPICS && buffer[i] != NULL; i++)
		free(buffer[i]);
	free(buffer);

	return 0;
}
