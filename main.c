#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "chess.h"

#define INITFEN "RNBQKBNR/PPPPPPPP/8/8/8/3p4/ppp1pppp/rnbqkbnr"
#define INPUTSIZE 5
#define MAXMOVES 512

#define TOPBOR  " |‾‾‾‾‾|‾‾‾‾‾|‾‾‾‾‾|‾‾‾‾‾|‾‾‾‾‾|‾‾‾‾‾|‾‾‾‾‾|‾‾‾‾‾|\n"
#define MIDBOR  " |     |     |     |     |     |     |     |     |\n"
#define BOTBOR  " |_____|_____|_____|_____|_____|_____|_____|_____|\n"
#define FILES   "    a     b     c     d     e     f     g     h\n"

void genboard(char *fen, int (*board)[BLEN]);
void printboard(int (*board)[BLEN]);
void takeinput(char *s);
void printmove(struct Move *move);

int main(int argc, char *argv[]) {
    CLS
    int board[BLEN][BLEN] = {{0}};
    char input[5];
    struct Move moves[MAXMOVES];
    int i;

    genboard(INITFEN, board);
    printboard(board);
    /*takeinput(input);*/
    struct Move *end = genallmoves(moves, board, true);
    for (i = 0; i < end - &(moves[0]); i++) {
        struct Move move = moves[i];
       printmove(&move);
    }

    printf("%s\n", input);
}

/* printmove: translate list indeces to chess ranks and files and print*/
void printmove(struct Move *move) {
    printf("%c%c%i %c%i\n", move->piece, move->xo+97, 8-move->yo, move->xf+97, 8-move->yf);
}

void takeinput(char *s) {
    char c;
    int i;
    printf("move: ");
    i = 0;
    while ((c = getchar()) != '\n')
        s[i++] = c;
    while (s[i] != '\0')
        s[i] = 0;
}

/* printboard: print a text-based chess board */
void printboard(int (*board)[BLEN]) {
    int y, x, c;
    printf("%s", TOPBOR);
    for (y = 0; y < BLEN; y++) {
        printf("%c|  ", 56-y);
        for (x = 0; x < BLEN; x++) {
            c = board[y][x];
            c == 0 ? putchar(' ') : putchar(c);
            printf("  |  ");
        }
        putchar('\n');
        printf("%s%s", BOTBOR, y != 7 ? MIDBOR : "");
    }
    printf("%s", FILES);
}

/* genboard: generate a board from a fen into *board */
void genboard(char *fen, int (*board)[BLEN]) {
    char c;
    int i, x, y;
    i = x = y = 0;
    while ((c = fen[i++]) != '\0') {
        if (isalpha(c))
            board[y][x++] = c;
        else if (isdigit(c))
            x += c - 48;
        else if (c == '/') {
            x = 0;
            y++;
        }
    }
}

