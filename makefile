default: FL1.c
	gcc -g FL1.c -o FL1.exe -Wall -std=c17
clean:
	$(RM) *.exe
	$(RM) *.stackdump