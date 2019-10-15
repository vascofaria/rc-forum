# _______________________________________________________________________________
# »»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
# IST - RC 2019/2020
#
# Makefile
#
# -------------------------------------------------------------------------------
#
# Made by: Bruno Meira (89421), Nikoletta Matsur (89513) and Vasco Faria (89559)
#
# «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««

# FLAGS= ANSI
NAME= server
Object= udp-manager/udp-manager.o udp-manager/register-io.o udp-manager/list-topics-io.o udp-manager/propose-topic-io.o udp-manager/list-questions-io.o tcp-manager/tcp-manager.o tcp-manager/question-get-io.o tcp-manager/question-submit-io.o tcp-manager/answer-submit-io.o file-manager/file-manager.o file-manager/topic-file-manager.o file-manager/question-file-manager.o file-manager/answer-file-manager.o entities/answer.o entities/question.o usecases/usecases.o
CC= gcc

all: $(Object)
	$(CC) -o $(NAME) main.c $(Object)

answer.o: entities/answer.c entities/answer.h
	$(CC) -c entities/answer.c

question.o: entities/question.c entities/question.h
	$(CC) -c entities/question.c

usecases.o: usecases/usecases.c usecases/usecases.h
	$(CC) -c usecases/usecases.c


file-manager.o: file-manager/file-manager.c file-manager/file-manager.h entities/question.h entities/answer.h exceptions.h constants.h
	$(CC) -c file-manager/file-manager.c

topic-file-manager.o: file-manager/topic-file-manager.c file-manager/topic-file-manager.h file-manager/file-manager.h entities/question.h entities/answer.h exceptions.h constants.h
	$(CC) -c file-manager/answer-file-manager.c

question-file-manager.o: file-manager/question-file-manager.c file-manager/question-file-manager.h file-manager/file-manager.h entities/question.h entities/answer.h exceptions.h constants.h
	$(CC) -c file-manager/answer-file-manager.c

answer-file-manager.o: file-manager/answer-file-manager.c file-manager/answer-file-manager.h file-manager/file-manager.h entities/question.h entities/answer.h exceptions.h constants.h
	$(CC) -c file-manager/answer-file-manager.c

udp-manager.o: udp-manager/udp-manager.c udp-manager/udp-manager.h exceptions.h constants.h protocol.h
	$(CC) -c udp-manager/udp-manager.c

register-io.o: udp-manager/register-io.c udp-manager/register-io.h exceptions.h constants.h protocol.h
	$(CC) -c udp-manager/register.c

list-topics-io.o: udp-manager/list-topics-io.c udp-manager/list-topics-io.h exceptions.h constants.h protocol.h
	$(CC) -c udp-manager/list-topics.c

propose-topic-io.o: udp-manager/propose-topic-io.c udp-manager/propose-topic-io.h exceptions.h constants.h protocol.h
	$(CC) -c udp-manager/propose-topic-io.c

list-questions-io.o: udp-manager/list-questions-io.c udp-manager/list-questions-io.h exceptions.h constants.h protocol.h
	$(CC) -c udp-manager/list-questions-io.c


tcp-manager.o: tcp-manager/tcp-manager.c tcp-manager/tcp-manager.h entities/question.h entities/answer.h exceptions.h constants.h protocol.h
	$(CC) -c tcp-manager/tcp-manager.c

question-get-io.o: tcp-manager/question-get-io.c tcp-manager/question-get-io.h entities/question.h entities/answer.h exceptions.h constants.h protocol.h
	$(CC) -c tcp-manager/question-get-io.c

question-submit-io.o: tcp-manager/question-submit-io.c tcp-manager/question-submit-io.h entities/question.h exceptions.h protocol.h constants.h
	$(CC) -c tcp-manager/question-submit-io.c

answer-submit-io.o: tcp-manager/answer-submit-io.c tcp-manager/answer-submit-io.h entities/question.h entities/answer.h protocol.h constants.h
	$(CC) -c tcp-manager/answer-submit-io.c


clean:
	@echo Cleaning Server...
	(rm -f entities/*.o)
	(rm -f usecases/*.o)
	(rm -f file-manager/*.o)
	(rm -f tcp-manager/*.o)
	(rm -f udp-manager/*.o)
	(rm -f *.o)
	(rm -f $(NAME))
