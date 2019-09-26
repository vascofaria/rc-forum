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
Object= main.o io-manager/udp-manager.o io-manager/tcp-manager.o file-manager.o
CC= gcc

file-manager.o: file-manager/file-manager.c file-manager/file-manager.h entities/question.h entities/answer.h exceptions.h constants.h
	$(CC) -c file-manager/file-manager.c

tcp-manager.o: io-manager/tcp-manager.c io-manager/tcp-manager.h entities/question.h entities/answer.h exceptions.h constants.h protocol.h
	$(CC) -c io-manager/tcp-manager.c

udp-manager.o: io-manager/udp-manager.c io-manager/udp-manager.h exceptions.h constants.h protocol.h
	$(CC) -c io-manager/udp-manager.c

main.o: main.c io-manager/udp-manager.h io-manager/tcp-manager.h constants.h
	$(CC) -c main.c

all:
	$(CC) -o $(NAME) $(Object)

clean:
	rm file-manager/*.o
	rm io-manager/*.o
	rm *.o
	rm server