#ifndef __USER_H__
#define __USER_H__

#include "../topic/topic.h"
#include "../vector/vector.h"
#include "../question/question.h"

typedef struct user {
	vector_t 	*topics;
	vector_t    *questions;
	char*    	user_id;
	char*    	topic;
	char*    	question;
	struct   	addrinfo *udp_addrinfo, *tcp_addrinfo;
	int      	server_sock_udp, server_sock_tcp;
} user_t;

user_t* 
create_user ();

char*
get_user_id (user_t *user);

char*
get_user_topic (user_t *user);

topic_t*
get_topic_from_topiclist(user_t *user, int topic_number);

char*
get_user_question (user_t *user);

vector_t*
get_user_topics(user_t *user);

vector_t*
get_user_questions(user_t *user);

struct addrinfo* 
get_user_udp_addrinfo(user_t *user);

struct addrinfo* 
get_user_tcp_addrinfo(user_t *user);

int 
get_user_server_sock_udp(user_t *user);

int 
get_user_server_sock_tcp(user_t *user);

void 
add_question_to_questionlist(user_t *user, char* question_name, char* question_user, char *answers_number);

void
add_existing_question_to_questionlist(user_t *user, question_t* question);

void 
add_topic_to_topiclist(user_t *user, char *topic_name, char *topic_number);

void
add_existing_topic_to_topiclist(user_t *user, topic_t* topic);

void 
set_user_id (user_t *user, char* id);

void 
set_user_topic (user_t* user, char* topic);

void 
set_user_question (user_t* user, char* question);

void 
set_user_topics(user_t* user, vector_t* topics);

void 
set_user_questions(user_t* user, vector_t* questions);

void 
set_user_udp_addrinfo(user_t *user, struct addrinfo *addrinfo);

void 
set_user_tcp_addrinfo(user_t *user, struct addrinfo *addrinfo);

void
set_user_server_sock_udp(user_t *user, int sock_fd);

void
set_user_server_sock_tcp(user_t *user, int sock_fd);

void 
clear_user_udp_addrinfo(user_t *user);

void 
clear_user_tcp_addrinfo(user_t *user);

void
erase_user (user_t *user);

#endif