#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "user.h"
#include "../constants.h"

user_t* 
create_user () {
	int i;
	user_t* user = (user_t*) malloc (sizeof(user_t));
	
	if (user != NULL) {
		user->user_id  = NULL;
		user->topic    = NULL;
		user->question = NULL;
		user->udp_addrinfo = NULL;
		user->tcp_addrinfo = NULL;
		user->topics    = vector_alloc(1, erase_topic);
		user->questions = vector_alloc(1, erase_question);
	}

	return user;
}

char*
get_user_id (user_t *user) {
	if (user) {
		return user->user_id;
	}

	return NULL;
}

char*
get_user_topic (user_t *user) {
	if (user) {
		return user->topic;	
	}

	return NULL;
}

topic_t*
get_topic_from_topiclist(user_t *user, int topic_number) {
	if (user && user->topics) {
		if (topic_number - 1 < 0 || topic_number - 1 >= user->topics->size) {
			return NULL;
		}
 		return (topic_t*) vector_at(user->topics, topic_number - 1);	
	}
	return NULL;
}

question_t*
get_question_from_questionlist(user_t *user, int question_number) {
	if (user && user->questions) {
		if (question_number - 1 < 0 || question_number - 1 >= user->questions->size) {
			return NULL;
		}
 		return (question_t*) vector_at(user->questions, question_number - 1);	
	}
	return NULL;
}


char*
get_user_question (user_t *user) {
	if (user) {
		return user->question;
	}
	return NULL;
}

vector_t*
get_user_topics(user_t *user) {
	if (user) {
		return user->topics;
	}
	return NULL;
}

vector_t*
get_user_questions(user_t *user) {
	if (user) {
		return user->questions;
	}
	return NULL;
}

struct addrinfo* 
get_user_udp_addrinfo(user_t *user) {
	if (user) {
		return user->udp_addrinfo;
	}
	return NULL;
}

struct addrinfo* 
get_user_tcp_addrinfo(user_t *user) {
	if (user) {
		return user->tcp_addrinfo;
	}
	return NULL;
}

int 
get_user_server_sock_udp(user_t *user) {
	if (user) {
		return user->server_sock_udp;
	}
	return -1;
}

int 
get_user_server_sock_tcp(user_t *user) {
	if (user) {
		return user->server_sock_tcp;
	}
	return -1;
}

void 
add_question_to_questionlist(user_t *user, char* question_name, char* question_user, char* answers_number) {
	question_t *question;

	if (user && user->questions && user->questions->size < MAX_QUESTIONS_NUMBER) {
		question = create_question(question_name, question_user, answers_number);
		if (question) {
			vector_pushBack(user->questions, question);
		}
	}
}

void
add_existing_question_to_questionlist(user_t *user, question_t* question) {
	if (user && user->questions && user->questions->size < MAX_QUESTIONS_NUMBER) {
		if (question) {
			vector_pushBack(user->questions, question);
		}
	}
}

void 
add_topic_to_topiclist(user_t *user, char* topic_name, char* topic_user) {
	topic_t *topic;

	if (user && user->topics && user->topics->size < MAX_TOPICS_NUMBER) {
		topic = create_topic(topic_name, topic_user);
		if (topic) {
			vector_pushBack(user->topics, topic);
		}
	}
}

void
add_existing_topic_to_topiclist(user_t *user, topic_t* topic) {
	if (user && user->topics && user->topics->size < MAX_TOPICS_NUMBER) {
		if (topic) {
			vector_pushBack(user->topics, topic);
		}
	}
}

void 
set_user_id (user_t *user, char* id) {
	if (user) {
		if (user->user_id) {
			free(user->user_id);
			user->user_id = NULL;
		}
		user->user_id = strdup(id);
	}
}

void 
set_user_topic(user_t* user, char* topic) {
	if (user) {
		if (user->topic) {
			free(user->topic);
			user->topic = NULL;
		}
		user->topic = strdup(topic);
	}
}

void 
set_user_question(user_t* user, char* question) {
	if (user) {
		if (user->question) {
			free(user->question);
			user->question = NULL;
		}
		user->question = strdup(question);
	}
}

void 
set_user_topics(user_t* user, vector_t* topics) {
	int i;

	if (user) {
		if (user->topics) {
			vector_free(user->topics);
		}
		
		user->topics = ((topics->size) > (MAX_TOPICS_NUMBER)) ? vector_alloc(MAX_TOPICS_NUMBER, erase_topic) : vector_alloc(topics->size, erase_topic);
		
		if (user->topics) {
			for (i = 0; i < topics->size; i++) {
				vector_pushBack(user->topics, vector_at(topics, i));
			}
		}
	}
}

void 
set_user_questions(user_t* user, vector_t* questions) {
	int i;

	if (user) {
		if (user->questions) {
			vector_free(user->questions);
		}
		
		user->questions = ((questions->size) > (MAX_QUESTIONS_NUMBER)) ? vector_alloc(MAX_QUESTIONS_NUMBER, erase_question) : vector_alloc(questions->size, erase_question);
		
		if (user->questions) {
			for (i = 0; i < questions->size; i++) {
				vector_pushBack(user->questions, vector_at(questions, i));
			}
		}
	}
}

void 
set_user_udp_addrinfo(user_t *user, struct addrinfo *addrinfo) {
	if (user) {
		user->udp_addrinfo = addrinfo;
	}
}

void 
set_user_tcp_addrinfo(user_t *user, struct addrinfo *addrinfo) {
	if (user) {
		user->tcp_addrinfo = addrinfo;
	}
}

void
set_user_server_sock_udp(user_t *user, int sock_fd) {
	if (user) {
		user->server_sock_udp = sock_fd;
	}
}

void
set_user_server_sock_tcp(user_t *user, int sock_fd) {
	if (user) {
		user->server_sock_tcp = sock_fd;
	}
}

void 
clear_user_udp_addrinfo(user_t *user) {
	if (user) {
		user->udp_addrinfo = NULL;
	}
}

void 
clear_user_tcp_addrinfo(user_t *user) {
	if (user) {
		user->tcp_addrinfo = NULL;
	}
}

void
erase_user (user_t *user) {
	int i;

	if (user) {
		if (user->user_id) {
			free(user->user_id);
			user->user_id = NULL;
		}

		if (user->topic) {
			free(user->topic);
			user->topic = NULL;
		}

		if (user->question) {	
			free(user->question);
			user->question = NULL;
		}

		if (user->topics != NULL) {
			vector_free(user->topics);
			free(user->topics);
			user->topics = NULL;
		}

		if (user->questions != NULL) {
			vector_free(user->questions);
			free(user->questions);
			user->questions = NULL;
		}

		free(user);
		user = NULL;
	}
}