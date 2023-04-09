#include "chess.h"
#include <SDL2/SDL_render.h>

#define INITFEN "RNBQKBNR/PPPPPPPP/8/8/8/8/pppppppp/rnbqkbnr"


int main(int argc, char *argv[]) {
    init();

    CLS
    struct Move moves[MAXMOVES];
    struct Move *move;
    int board[BLEN][BLEN] = {{0}};
    char input[INPUTSIZE];
    int i, nummoves;

    extern SDL_Window *window;
    extern SDL_Renderer *renderer;
    extern SDL_Surface *screensurface;

    genboard(INITFEN, board);

    i = 0;
    SDL_Event event;
    SDL_Texture *texture;
    do {
        SDL_PollEvent(&event);
        SDL_RenderClear(renderer);
        renderboard(screensurface, board);
        texture = SDL_CreateTextureFromSurface(renderer, screensurface);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_Delay(20);


        struct Move *end = genallmoves(moves, board, (i % 2 == 0), true);
        nummoves = end - &(moves[0]);
        if (nummoves == 0) {
            printf("Checkmate, %s wins!\n", (i % 2 == 0) ? "black" : "white");
            exit(0);
        }

        while (true) {
            printf("%d moves\n", nummoves);
            takeinput(input);
            move = parseinput(input, moves, nummoves);
            if (move == NULL)
                printf("\033[F\33[2K\033[F");
            else
                break;
        }
        makemove(move, board);
        CLS
        i++;
    } while (!(event.key.keysym.sym == SDLK_ESCAPE));

    SDL_DestroyWindow(window);
    SDL_Quit();
}

