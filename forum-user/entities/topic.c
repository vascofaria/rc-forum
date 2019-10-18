#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "topic.h"

topic_t*
create_topic(char *topic_name, char *topic_user) {
	topic_t *topic = (topic_t *) malloc (sizeof(topic_t));

	if (topic) {
		topic->topic_name = strdup(topic_name);
		topic->topic_user = strdup(topic_user);
	}

	return topic;
}

char*
get_topic_name(topic_t *topic) {
	if (topic) {
		return topic->topic_name;
	}
	return NULL;
}

char*
get_topic_user(topic_t *topic) {
	if (topic) {
		return topic->topic_user;
	}
	return NULL;
}

void
set_topic_name(topic_t *topic, char* topic_name) {
	if (topic) {
		if (topic->topic_name) {
			free(topic->topic_name);
			topic->topic_name = NULL;
		}
		topic->topic_name = strdup(topic_name);
	}
}

void
set_topic_user(topic_t *topic, char* topic_user) {
	if (topic) {
		if (topic->topic_user) {
			free(topic->topic_user);
			topic->topic_user = NULL;
		}
		topic->topic_user = strdup(topic_user);
	}
}

void
erase_topic(void *topic) {
	topic_t *t = (topic_t*) topic;
	if (t) {
		if (t->topic_name) {
			free(t->topic_name);
			t->topic_name = NULL;
		}
		if (t->topic_user) {
			free(t->topic_user);
			t->topic_user = NULL;
		}

		free(t);
		t = NULL;
	}
}