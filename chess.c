#include <stdio.h>
#include "chess.h"

struct Move newmove(int yo, int xo, int yf, int xf, char piece);
bool isopponent(char piece, char target);
struct Move* genpiecemoves(struct Move *moves, int yo, int xo, int (*board)[BLEN]);
struct Move* pawnmoves(struct Move *moves, int yo, int xo, int (*board)[BLEN]);
struct Move* knightmoves(struct Move *moves, int yo, int xo, int (*board)[BLEN]);
struct Move* bishopmoves(struct Move *moves, int yo, int xo, int (*board)[BLEN]);
struct Move* rookmoves(struct Move *moves, int yo, int xo, int (*board)[BLEN]);
struct Move* queenmoves(struct Move *moves, int yo, int xo, int (*board)[BLEN]);
struct Move* kingmoves(struct Move *moves, int yo, int xo, int (*board)[BLEN]);


struct Move newmove(int yo, int xo, int yf, int xf, char piece) {
    struct Move move = { yo, xo, yf, xf, piece };
    return move;
}

bool isopponent(char piece, char target) {
    return (isupper(piece) && islower(target)) || (islower(piece) && isupper(target));
}

void genallmoves(struct Move *moves, int (*board)[BLEN], bool white) {
    int x, y;
    if (white) {
        for (y = 0; y < BLEN; y++)
            for (x = 0; x < BLEN; x++)
                if (islower(board[y][x])) {
                    moves = genpiecemoves(moves, y, x, board);
                }
    }
}

struct Move* genpiecemoves(struct Move *moves, int yo, int xo, int (*board)[BLEN]) {
    switch (board[yo][xo]) {
        case 'p':
        case 'P':
            moves = pawnmoves(moves, yo, xo, board);
            break;
        case 'n':
        case 'N':
            moves = knightmoves(moves, yo, xo, board);
            break;
        case 'b':
        case 'B':
            moves = bishopmoves(moves, yo, xo, board);
            break;
        case 'r':
        case 'R':
            moves = rookmoves(moves, yo, xo, board);
            break;
        case 'q':
        case 'Q':
            moves = queenmoves(moves, yo, xo, board);
            break;
        case 'k':
        case 'K':
            moves = kingmoves(moves, yo, xo, board);
            break;
    }
    return moves;
}

struct Move* pawnmoves(struct Move *moves, int yo, int xo, int (*board)[BLEN]) {
    bool moved = false;
    char piece = board[yo][xo];
    int so, to;
    so = 1;
    to = 2;
    if (islower(piece)) {
        so *= -1;
        to *= -1;
    }
    int i = 0;
    moved = (islower(piece) && yo != 6) || (isupper(piece) && yo != 1);
    if ((board[yo+to][xo] == 0) && !moved)
        moves[i++] = newmove(yo, xo, yo+to, xo, piece);
    if (board[yo+so][xo] == 0)
        moves[i++] = newmove(yo, xo, yo+so, xo, piece);
    if (xo > 0)
        if (isopponent(piece, board[yo+so][xo-1]))
            moves[i++] = newmove(yo, xo, yo+so, xo - 1, piece);
    if (xo < 7)
        if (isopponent(piece, board[yo+so][xo+1]))
            moves[i++] = newmove(yo, xo, yo+so, xo + 1, piece);
    return &moves[i];
}

struct Move* knightmoves(struct Move *moves, int yo, int xo, int (*board)[BLEN]) {
    return moves;
}
struct Move* bishopmoves(struct Move *moves, int yo, int xo, int (*board)[BLEN]) {
    return moves;
}
struct Move* rookmoves(struct Move *moves, int yo, int xo, int (*board)[BLEN]) {
    return moves;
}
struct Move* queenmoves(struct Move *moves, int yo, int xo, int (*board)[BLEN]) {
    return moves;
}
struct Move* kingmoves(struct Move *moves, int yo, int xo, int (*board)[BLEN]) {
    return moves;
}
