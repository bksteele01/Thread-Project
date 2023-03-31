CC = gcc
CFLAGS = -g -Wall -pthread

default: thread

thread: main.o msgq.o
	$(CC) $(CFLAGS) -o thread main.o msgq.o -lm
main.o: main.c
	$(CC) $(CFLAGS) -c main.c
msgq.o: msgq.c
	$(CC) $(CFLAGS) -c msgq.c

clean:
	rm -f thread *.o
