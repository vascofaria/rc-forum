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

NAME= client
Object= protocol-manager/client-manager.o protocol-manager/client-tcp-manager.o protocol-manager/client-udp-manager.o user/user.o topic/topic.o vector/vector.o file-manager/file-manager.o
CC= gcc

all: $(Object)
	$(CC) -o $(NAME) main.c $(Object)


file-manager.o: file-manager/file-manager.c file-manager/file-manager.h
	$(CC) -c file-manager/file-manager.c

client-manager.o: protocol-manager/client-manager.c protocol-manager/client-manager.h user/user.h
	$(CC) -c protocol-manager/client-manager.c

client-tcp-manager.o: protocol-manager/client-tcp-manager.c protocol-manager/client-tcp-manager.h user/user.h
	$(CC) -c protocol-manager/client-tcp-manager.c

client-udp-manager.o: protocol-manager/client-udp-manager.c protocol-manager/client-udp-manager.h user/user.h
	$(CC) -c protocol-manager/client-udp-manager.c

user.o: user/user.c user/user.h
	$(CC) -c user/user.c

topic.o: topic/topic.c topic/topic.h
	$(CC) -c topic/topic.c

vector.o: vector/vector.c vector/vector.h
	$(CC) -c vector/vector.c

clean:
	@echo Cleaning Client...
	(rm -f file-manager/*.o;)
	(rm -f protocol-manager/*.o;)
	(rm -f user/*.o;)
	(rm -f vector/*.o;)
	(rm -f topic/*.o;)
	(rm -f *.o;)
	(rm -f $(NAME);)