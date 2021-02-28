CC = gcc
FLAGS =-g -std=c17

all: testlist

testlist: main.c list.c
	$(CC) $(FLAGS) main.c list.c -o testlist -I.
	
clean:
	rm testlist