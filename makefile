
objects := $(patsubst src%, target%, $(patsubst %.c,%.o,$(wildcard src/*.c)))
headers := $(wildcard src/*.h)

chess : $(objects) $(headers)
	gcc $(objects) $(headers) -o target/chess -lSDL2_image $$(sdl2-config --libs)

target/main.o : src/main.c src/chess.h
	gcc -c src/main.c -o target/main.o

target/chess.o : src/chess.c src/chess.h
	gcc -c src/chess.c -o target/chess.o

target/lib.o : src/lib.c src/chess.h
	gcc -c src/lib.c -o target/lib.o

target/gui.o : src/gui.c src/chess.h
	gcc -c src/gui.c $$(sdl2-config --cflags) -o target/gui.o

clean :
	rm target/*
