#ifndef __TOPIC_H__
#define __TOPIC_H__

typedef struct topic {
	char *topic_name;
	char *topic_user;
} topic_t;

topic_t*
create_topic(char *topic_name, char *topic_user);

char*
get_topic_name(topic_t *topic); 

char*
get_topic_user(topic_t *topic);

void
set_topic_name(topic_t *topic, char* topic_name);

void
set_topic_user(topic_t *topic, char* topic_user);

void
erase_topic(void *topic);
#endif