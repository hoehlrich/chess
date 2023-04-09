#include "chess.h"

#define TOPBOR  " |‾‾‾‾‾|‾‾‾‾‾|‾‾‾‾‾|‾‾‾‾‾|‾‾‾‾‾|‾‾‾‾‾|‾‾‾‾‾|‾‾‾‾‾|\n"
#define MIDBOR  " |     |     |     |     |     |     |     |     |\n"
#define BOTBOR  " |_____|_____|_____|_____|_____|_____|_____|_____|\n"
#define FILES   "    a     b     c     d     e     f     g     h\n"

/* parseinput: find move that matches input; returns pointer to the best
 * matching move */
struct Move* parseinput(char *s, struct Move *moves, int nummoves) {
    int i, yf, xf, yo, xo, max;
    char c, piece;
    yf = xf = yo = xo = -1;
    piece = -1;
    max = INPUTSIZE-1;
    struct Move move;
    for ( i = INPUTSIZE-1; i >= 0; i--) {
        c = s[i];
        if (c == 0)
            continue;
        else if (isdigit(c)) {
            max = i;
            yf = 7 - (c - 49);
        } else if (isalpha(c)) {
            if (xf == -1)
                xf = c - 97;
            else if (i == 0 && max > 1)
                piece = c;
            else
                xo = c - 97;
        }
    }
    for (i = 0; i < nummoves; i++) {
        move = moves[i];
        if (move.xf == xf && move.yf == yf)
            if (((piece == -1) && (tolower(move.piece) == 'p')) || move.piece == piece)
                if (xo == -1 || move.xo == xo)
                    return &(moves[i]);
    }
    return NULL;
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
