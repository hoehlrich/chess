#include "chess.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>

#define INITFEN "RNBQKBNR/PPPPPPPP/8/8/8/8/pppppppp/rnbqkbnr"


int main(int argc, char *argv[]) {
    init();

    struct Move moves[MAXMOVES];
    struct Move *move;
    int board[BLEN][BLEN] = {{0}};
    char input[INPUTSIZE+1], key;
    int i, j, nummoves, ip;

    input[INPUTSIZE] = '\0';

    extern SDL_Window *window;
    extern SDL_Renderer *renderer;
    extern SDL_Surface *screensurface;

    genboard(INITFEN, board);

    i = ip = 0;
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

        /* Handle input */
        key = event.key.keysym.sym;
        if (event.type != SDL_KEYDOWN) {
            ;
        } else if (key == SDLK_RETURN) {
            ip = 0;
            move = parseinput(input, moves, nummoves);
            if (move != NULL) {
                makemove(move, board);
                i++;
            } else
                printf("%s does not exist\n", input);
            clrstr(input);
        } else if ((isalpha(key) && islower(key)) || isdigit(key)) {
            if (ip == INPUTSIZE) {
                clrstr(input);
                ip = 0;
            } else {
                input[ip++] = key;
            }
        }
    } while (!(event.key.keysym.sym == SDLK_ESCAPE));

    SDL_DestroyWindow(window);
    SDL_Quit();
}
