CC= gcc
CFLAGS= -Wall -Wextra -g

cmatrix: main.c cmatrix.h
	$(CC) $(CFLAGS) main.c -o cmatrix.out -lncurses
	rm -f *.o

clean:
	rm -f cmatrix *.o

.PHONY: clean
