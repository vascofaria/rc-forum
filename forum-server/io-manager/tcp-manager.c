

#include "tcp-manager.h"
// #include "protocol.h"
// #include "../entities/question.h"
// #include "../exceptions.h"


// case GQU
// char topic[MAXNAME]
// char question[MAXNAME]
// parse_i_GQU(request, topic, question)
// if error_code return parse_output_ERROR()
// else
// error_code = question_get(topic, question)
// if error_code parse_output_ERROR(error_code) from exceptions.h
char *tcp_manager(char *request) {
	// switch

	// case GQU
	// parse_input_GQU, question_get, parse_output_QGR

	// case QUS
	// parse_input_QUS, question_submit, parse_output_QUR

	// case ANS
	// parse_input_ANS, answer_submit, parse_output_ANR

	// case default
	// parse_output_ERROR
	return NULL;
}

/*
 * QUESTION GET
*/
static int parse_input_GQU(char *request, char *topic, char *question) {
	// check the request string
	return 0;
}

static char *parse_output_QGR() { // question
	// parse the question to the protocol string
	return NULL;
}

/*
 * QUESTION SUBMIT
*/
static int parse_input_QUS(char *request) {
	// check the request string
	return 0;
}

static char *parse_output_QUR() {
	// make the success protocol string
	return NULL;
}

/*
 * ANSWER SUBMIT
*/
static int parse_input_ANS(char *request) {
	// check the request string
	return 0;
}

static char *parse_output_ANR() {
	// make the success protocol string
	return NULL;
}

/*
 * ERROR PROTOCOL
*/
static char *parse_output_ERROR(int error_code) {
	// check wich error and turn it into the respective protocol error
	return NULL;
}
