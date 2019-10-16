/* _______________________________________________________________________________
 * »»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
 * IST - RC 2019/2020
 *
 * file-manager.h
 *
 * -------------------------------------------------------------------------------
 *
 * Made by: Bruno Meira (89421), Nikoletta Matsur (89513) and Vasco Faria (89559)
 *
 * «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««
 */

#ifndef  __FILE_MANAGER_H__
#define  __FILE_MANAGER_H__

#include "../exceptions.h"
#include "../entities/question.h"
#include "../entities/answer.h"

#define MAX_PATH      90
#define MAX_FILENAME  20

#define TOPICS_PATH    "./topics/\0"
#define QUESTIONS_PATH "/questions/\0"
#define ANSWERS_PATH   "/answers/\0"


int file_exists(char *file_path);

/* ===============================================================================
 * char **list_directory(char* path);
 * -------------------------------------------------------------------------------
 * Lists all the files ignoring the ones that start with '.'
 * -------------------------------------------------------------------------------
 * RETURN a pointer to an allocated strings list
 * ===============================================================================
*/
char **list_directory(char* path);

int move_directory(char *path);

int get_file_size(const char *file_name, const char* mode);

char* get_file_data(const char* file_name, const char* mode);

void write_file_data(const char *file_path, const long int size, const char *data);

char *get_img_file(char *path);

char* get_img_ext(char* path);

int count_directories(char *path);

int write_from_socket_to_file(int sock_tcp, char *file_path, int file_size);

int write_from_file_to_socket(int sock_tcp, char *file_path, int file_size);

#endif