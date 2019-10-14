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
Object= protocol-manager/udp-manager.o protocol-manager/tcp-manager.o file-manager/file-manager.o entities/answer.o entities/question.o usecases/usecases.o
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

tcp-manager.o: protocol-manager/tcp-manager.c protocol-manager/tcp-manager.h entities/question.h entities/answer.h exceptions.h constants.h protocol.h
	$(CC) -c protocol-manager/tcp-manager.c

udp-manager.o: protocol-manager/udp-manager.c protocol-manager/udp-manager.h exceptions.h constants.h protocol.h
	$(CC) -c protocol-manager/udp-manager.c


clean:
	@echo Cleaning Server...
	(rm -f entities/*.o)
	(rm -f usecases/*.o)
	(rm -f file-manager/*.o)
	(rm -f protocol-manager/*.o)
	(rm -f *.o)
	(rm -f $(NAME))
