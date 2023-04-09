chess : main.o chess.o text.o gui.o
	gcc main.o chess.o text.o gui.o -o chess -lSDL2_image $$(sdl2-config --libs)

main.o : main.c
	gcc -c main.c

chess.o : chess.c
	gcc -c chess.c

text.o : text.c
	gcc -c text.c

gui.o : gui.c
	gcc -c gui.c $$(sdl2-config --cflags)
