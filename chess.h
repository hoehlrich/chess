#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define CLS printf("\e[1;1H\e[2J");
#define BLEN 8
#define MAXMOVES 512
#define INPUTSIZE 5

struct Move {
    int yo;
    int xo;
    int yf;
    int xf;
    char piece;
};

void genboard(char *fen, int (*board)[BLEN]);
struct Move* genallmoves(struct Move *moves, int (*board)[BLEN], bool white, bool checkcheck);
void makemove(struct Move *move, int (*board)[BLEN]);
struct Move* parseinput(char *s, struct Move *moves, int nummoves);
void printboard(int (*board)[BLEN]);
void printmove(struct Move *move);
void takeinput(char *s);
void renderboard(SDL_Surface *surface, int (*board)[BLEN]);
char keyboard_input(SDL_Event *event);
int init();

