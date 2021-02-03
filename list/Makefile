CC = gcc
FLAGS =-g -std=c11

all: testlist

testlist: main.c list.c
	$(CC) $(FLAGS) main.c list.c -o testlist -I.
clean:
	rm testlist