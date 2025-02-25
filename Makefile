CC=cc
IN=src/shine.c
OUT=shine

install:
	$(CC) $(IN) -o $(OUT)
