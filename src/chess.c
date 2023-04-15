#include <stdio.h>
#include "chess.h"

#define LEN(x) (sizeof(x) / sizeof(x[0]))

struct Move* genpiecemoves(struct Move *moves, int yo, int xo, int (*board)[BLEN]);
struct Move* slidingmoves(struct Move *moves, int yo, int xo, int (*board)[BLEN]);
struct Move* knightmoves(struct Move *moves, int yo, int xo, int (*board)[BLEN]);
struct Move* pawnmoves(struct Move *moves, int yo, int xo, int (*board)[BLEN]);
struct Move* kingmoves(struct Move *moves, int yo, int xo, int (*board)[BLEN]);
struct Move newmove(int yo, int xo, int yf, int xf, char piece);
bool checking(struct Move *moves, int nummoves, int (*board)[BLEN], bool white);
bool insideboard(int y, int x);
bool isopponent(char piece, char target);
void unmakemove(struct Move *move, int (*board)[BLEN], int op);

static const int yd[] = { 1, 1, -1, -1, 1, 0, -1, 0 };
static const int xd[] = { 1, -1, 1, -1, 0, 1, 0, -1 };

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

/* checking: given moves, can the white/black king be taken? (in check)*/
bool checking(struct Move *moves, int nummoves, int (*board)[BLEN], bool white) {
    int i;
    struct Move move;
    for (i = 0; i < nummoves; i++) {
        move = moves[i];
        if (white) {
            if (board[move.yf][move.xf] == 'k')
                return true;
        } else {
            if (board[move.yf][move.xf] == 'K')
                return true;
        }
    }

    return false;
}

/* makemove: execute move on board */
void makemove(struct Move *move, int (*board)[BLEN]) {
    board[move->yo][move->xo] = 0;
    board[move->yf][move->xf] = move->piece;
}

/* unmakemove: undo move; op is original piece on the target square */
void unmakemove(struct Move *move, int (*board)[BLEN], int op) {
    board[move->yo][move->xo] = move->piece;
    board[move->yf][move->xf] = op;
}

/* newmove: constructor for Move */
struct Move newmove(int yo, int xo, int yf, int xf, char piece) {
    struct Move move = { yo, xo, yf, xf, piece };
    return move;
}

/* isopponent: is piece an opponent of target? */
bool isopponent(char piece, char target) {
    return (isupper(piece) && islower(target)) || (islower(piece) && isupper(target));
}

bool insideboard(int y, int x) {
    return (y < 8 && y >= 0 && x < 8 && x >= 0);
}

/* genallmoves: generate all moves for a player on top of *move. Returns a
 * pointer to the last element */
struct Move* genallmoves(struct Move *moves, int (*board)[BLEN], bool white, bool checkcheck) {
    int x, y;
    struct Move *initmoves = moves;
    for (y = 0; y < BLEN; y++)
        for (x = 0; x < BLEN; x++)
            if ((islower(board[y][x]) && white) || (isupper(board[y][x]) && !white))
                moves = genpiecemoves(moves, y, x, board);

    /* break if not checking for checks (prevents infinite recursion */
    if (!checkcheck)
        return moves;
    
    /* remove moves that will result in a self-check */
    int i, nummoves;
    int op;
    struct Move move;
    struct Move testmoves[MAXMOVES];

    struct Move *testmovesp = &(testmoves[0]);
    struct Move *testmovesinit = testmoves;
    nummoves = moves - initmoves;
    for (i = 0; i < nummoves; i++) {
        move = initmoves[i];
        op = board[move.yf][move.xf];
        makemove(&move, board);
        testmovesp = genallmoves(testmoves, board, !white, false);
        if (checking(testmovesinit, testmovesp - testmovesinit, board, white)) {
            initmoves[i] = *moves;
            moves -= 1;
        }
        unmakemove(&move, board, op);
        
    }
    return moves;
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
    if ((board[yo+to][xo] == 0) && !moved && (board[yo+so][xo] == 0))
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
            if (board[yf][xf] == 0) {
                moves[i++] = newmove(yo, xo, yf, xf, piece);
            } else if (isopponent(piece, board[yf][xf])) {
                moves[i++] = newmove(yo, xo, yf, xf, piece);
                break;
            } else {
                break;
            }
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
