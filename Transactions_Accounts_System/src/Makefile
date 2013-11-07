CC = gcc
CFLAGS = -m32 -Wall -Werror -g

buxfer: transactions_manager.o lists.o lists.h
	$(CC) $(CFLAGS) -o transactions_manager buxfer.o lists.o

buxfer.o: transactions_manager.c lists.h
	$(CC) $(CFLAGS) -c buxfer.c

lists.o: lists.c lists.h
	$(CC) $(CFLAGS) -c lists.c

clean: 
	rm transactions_manager *.o
