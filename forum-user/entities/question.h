#ifndef __question_H__
#define __question_H__

typedef struct question {
	char *question_name;
	char *question_user;
	char *answers_number;
} question_t;

question_t*
create_question(char *question_name, char *question_user, char* answers_number);

char*
get_question_name(question_t *question); 

char*
get_question_user(question_t *question);

char*
get_question_answers_number(question_t *question);

void
set_question_name(question_t *question, char* question_name);

void
set_question_user(question_t *question, char* question_user);

void
erase_question(void *question);
#endif