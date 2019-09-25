#include <udp-manager.h>

#define PROTOCOL_SIZE 3
#define USER_ID_SIZE  5
#define TOPIC_SIZE    10

static int parse_input_REG(char* request, char* user_id) {
	int len = strlen(request);

	if (len != 9 || request[PROTOCOL_SIZE] != ' ') {
		return 0;
	}

	for (i = PROTOCOL_SIZE + 1; i < n && request[i] != '\0' && request[i] != '\n'; i++) {
		if (request[i] < '0' || request[i] > '9') {
			return 0;
		}	
	}

	return 1;
}

static int parse_input_LTP(char* request) {

}

static int parse_input_PTP(char* request, char* user_id, char* topic) {

}

static int parse_input_LQU(char* request, char* topic) {
	
}

char* udp_manager (char *request) {
	char protocol[PROTOCOL_SIZE + 1];
	char user_id[USER_ID_SIZE + 1];
	char topic[TOPIC_SIZE + 1];

	for (i = 0; i < PROTOCOL_SIZE && request[i] != '\0' && request[i] != '\n';  i++) {
		protocol[i] = request[i];
	}

	protocol[3] = '\0';

	if (!strcmp(protocol, "REG")) {
		parse_input_reg(request, user_id);
	}

	/* TO DO: acabar o resto para os restantes protocolos*/
}