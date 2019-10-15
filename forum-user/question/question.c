#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "question.h"

question_t*
create_question(char *question_name, char *question_user, char* answers_number) {
	question_t *question = (question_t *) malloc (sizeof(question_t));

	if (question) {
		question->question_name  = strdup(question_name);
		question->question_user  = strdup(question_user);
		question->answers_number = strdup(answers_number);	
	}

	return question;
}

char*
get_question_name(question_t *question) {
	if (question) {
		return question->question_name;
	}
	return NULL;
}

char*
get_question_user(question_t *question) {
	if (question) {
		return question->question_user;
	}
	return NULL;
}

char*
get_question_answers_number(question_t *question) {
	if (question) {
		return question->answers_number;
	}
	return NULL;
}

void
set_question_name(question_t *question, char* question_name) {
	if (question) {
		if (question->question_name) {
			free(question->question_name);
			question->question_name = NULL;
		}
		question->question_name = strdup(question_name);
	}
}

void
set_question_user(question_t *question, char* question_user) {
	if (question) {
		if (question->question_user) {
			free(question->question_user);
			question->question_user = NULL;
		}
		question->question_user = strdup(question_user);
	}
}

void
set_question_answers_number(question_t *question, char* answers_number) {
	if (question) {
		if (question->answers_number) {
			free(question->answers_number);
			question->answers_number = NULL;
		}
		question->answers_number = strdup(answers_number);
	}
}

void
erase_question(void *question) {
	question_t *t = (question_t*) question;
	if (t) {
		if (t->question_name) {
			free(t->question_name);
			t->question_name = NULL;
		}
		if (t->question_user) {
			free(t->question_user);
			t->question_user = NULL;
		}
		if (t->answers_number) {
			free(t->answers_number);
			t->answers_number = NULL;
		}

		free(t);
		t = NULL;
	}
}