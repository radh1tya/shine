CC=cc
IN=shine.c
OUT=shine
CFLAGS=-Wall -Wextra -std=c99 -pedantic

install:
	$(CC) $(CFLAGS) $(IN) -o $(OUT)

remove:
	rm $(OUT)

fresh:
	rm $(OUT)
	$(CC) $(CFLAGS) $(IN) -o $(OUT)
