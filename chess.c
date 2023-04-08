#include <stdio.h>
#include "chess.h"

#define LEN(x) (sizeof(x) / sizeof(x[0]))

struct Move newmove(int yo, int xo, int yf, int xf, char piece);
bool isopponent(char piece, char target);
bool insideboard(int y, int x);
struct Move* genpiecemoves(struct Move *moves, int yo, int xo, int (*board)[BLEN]);
struct Move* pawnmoves(struct Move *moves, int yo, int xo, int (*board)[BLEN]);
struct Move* knightmoves(struct Move *moves, int yo, int xo, int (*board)[BLEN]);
struct Move* slidingmoves(struct Move *moves, int yo, int xo, int (*board)[BLEN]);
struct Move* rookmoves(struct Move *moves, int yo, int xo, int (*board)[BLEN]);
struct Move* queenmoves(struct Move *moves, int yo, int xo, int (*board)[BLEN]);
struct Move* kingmoves(struct Move *moves, int yo, int xo, int (*board)[BLEN]);

static const int yd[] = { 1, 1, -1, -1, 1, 0, -1, 0 };
static const int xd[] = { 1, -1, 1, -1, 0, 1, 0, -1 };


struct Move newmove(int yo, int xo, int yf, int xf, char piece) {
    struct Move move = { yo, xo, yf, xf, piece };
    return move;
}

bool isopponent(char piece, char target) {
    return (isupper(piece) && islower(target)) || (islower(piece) && isupper(target));
}

bool insideboard(int y, int x) {
    return (y < 8 && y >= 0 && x < 8 && x >= 0);
}

/* genallmoves: generate all moves for a player on top of *move. Returns a
 * pointer to the last element */
struct Move* genallmoves(struct Move *moves, int (*board)[BLEN], bool white) {
    int x, y;
    if (white) {
        for (y = 0; y < BLEN; y++)
            for (x = 0; x < BLEN; x++)
                if (islower(board[y][x])) {
                    moves = genpiecemoves(moves, y, x, board);
                }
    }
}

/* genpiecemoves: generate all moves for a piece on top of *move. Returns a
 * pointer to the last element */
struct Move* genpiecemoves(struct Move *moves, int yo, int xo, int (*board)[BLEN]) {
    switch (tolower(board[yo][xo])) {
        case 'p':
            moves = pawnmoves(moves, yo, xo, board);
            break;
        case 'n':
            moves = knightmoves(moves, yo, xo, board);
            break;
        case 'b':
        case 'r':
        case 'q':
            moves = slidingmoves(moves, yo, xo, board);
            break;
        case 'k':
            moves = kingmoves(moves, yo, xo, board);
            break;
    }
    return moves;
}

/* panmoves: generate all pawn moves on top move *moves. Returns a pointer to
 * the last element */
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

/* knightmoves: generate all knight moves on top of *moves. Returns a pointer to
 * the last element */
struct Move* knightmoves(struct Move *moves, int yo, int xo, int (*board)[BLEN]) {
    char piece = board[yo][xo];
    int yd[] = { 2, 2, 1, -1, -2, -2, 1, -1 };
    int xd[] = { 1, -1, -2, -2, -1, 1, 2, 2 };
    int i, j, yf, xf;
    i = 0;
    for (j = 0; j < LEN(yd); j++) {
        yf = yd[j] + yo;
        xf = xd[j] + xo;
        if (insideboard(yf, xf))
            if (board[yf][xf] == 0 || isopponent(piece, board[yf][xf]))
                moves[i++] = newmove(yo, xo, yf, xf, piece);
    }
    return &moves[i];
}

/* slidingmoves: generate all sliding piece moves on top of *moves. Returns a
 * pointer to the last element */
struct Move* slidingmoves(struct Move *moves, int yo, int xo, int (*board)[BLEN]) {
    char piece = board[yo][xo];
    int i, j, start, end, yf, xf;
    i = 0;
    switch (tolower(piece)) {
        case 'b':
            start = 0;
            end = 4;
            break;
        case 'r':
            start = 4;
            end = 8;
            break;
        case 'q':
            start = 0;
            end = 8;
            break;
    }

    for (j = start; j < end; j++) {
        yf = yo + yd[j];
        xf = xo + xd[j];
        while (insideboard(yf, xf)) {
            if (board[yf][xf] == 0 || isopponent(piece, board[yf][xf]))
                moves[i++] = newmove(yo, xo, yf, xf, piece);
            else
                break;
            yf += yd[j];
            xf += xd[j];
        }
    }
    return &moves[i];
}

/* kingmoves: generate all king moves. Returns a pointer to the last element */
struct Move* kingmoves(struct Move *moves, int yo, int xo, int (*board)[BLEN]) {
    char piece = board[yo][xo];
    int i, j, yf, xf;

    for (j = 0; j < LEN(yd); j++) {
        yf = yo + yd[j];
        xf = xo + xd[j];
        if (insideboard(yf, xf))
            if (board[yf][xf] == 0 || isopponent(piece, board[yf][xf]))
                moves[i++] = newmove(yo, xo, yf, xf, piece);
    }
    return &moves[i];
}
