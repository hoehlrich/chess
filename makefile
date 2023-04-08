chess : main.o chess.o
	gcc main.o chess.o -o chess -lc

main.o : main.c
	gcc -c main.c

chess.o : chess.c
	gcc -c chess.c
