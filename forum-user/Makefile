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

NAME= user
Object= protocol-manager/client-manager.o protocol-manager/client-tcp-manager.o protocol-manager/client-udp-manager.o entities/user.o entities/topic.o entities/question.o entities/vector.o file-manager/file-manager.o
CC= gcc

all: $(Object)
	$(CC) -o $(NAME) main.c $(Object)


file-manager.o: file-manager/file-manager.c file-manager/file-manager.h
	$(CC) -c file-manager/file-manager.c

client-manager.o: protocol-manager/client-manager.c protocol-manager/client-manager.h entities/user.h
	$(CC) -c protocol-manager/client-manager.c

client-tcp-manager.o: protocol-manager/client-tcp-manager.c protocol-manager/client-tcp-manager.h entities/user.h
	$(CC) -c protocol-manager/client-tcp-manager.c

client-udp-manager.o: protocol-manager/client-udp-manager.c protocol-manager/client-udp-manager.h entities/user.h
	$(CC) -c protocol-manager/client-udp-manager.c

user.o: entities/user.c entities/user.h
	$(CC) -c entities/user.c

topic.o: entities/topic.c entities/topic.h
	$(CC) -c entities/topic.c

question.o: entities/question.c entities/question.h
	$(CC) -c entities/question.c

vector.o: entities/vector.c entities/vector.h
	$(CC) -c entities/vector.c

clean:
	@echo Cleaning Client...
	(rm -f file-manager/*.o;)
	(rm -f protocol-manager/*.o;)
	(rm -f entities/*.o;)
	(rm -f *.o;)
	(rm -f $(NAME);)