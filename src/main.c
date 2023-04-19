#include "chess.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>

#define INITFEN "RNBQKBNR/PPPPPPPP/8/8/8/8/pppppppp/rnbqkbnr"

int main(int argc, char *argv[]) {
    /* Intialize SDL */
    init();
    extern SDL_Window *window;
    extern SDL_Renderer *renderer;
    extern SDL_Surface *screensurface;
    SDL_Event event;
    SDL_Texture *texture;

    /* Initialize board */
    int board[BLEN][BLEN] = {{0}};
    genboard(INITFEN, board);

    /* Initialize input */
    char input[INPUTSIZE+1], key;
    input[INPUTSIZE] = '\0';
    int inputp = 0;

    /* Initialize moves */
    struct CastleState castlestate = { false, false, false, false, false, false};
    struct Move moves[MAXMOVES];
    struct Move *move;
    struct Move *end = genallmoves(moves, board, true, true, &castlestate);
    int nummoves = end - &(moves[0]);

    /* Initialize loop vars */
    bool white = true;
    bool whitepov = true;

    /* Initial render */
    blitboard(screensurface, board, true);
    renderboard(renderer, screensurface, texture);

    do {
        SDL_PollEvent(&event);
        SDL_Delay(20);

        /* Handle input */
        key = event.key.keysym.sym;
        if (event.type != SDL_KEYDOWN) {
            ;
        } else if (key == SDLK_RETURN) {
            inputp = 0;
            move = parseinput(input, moves, nummoves);

            if (move == NULL) {
                printf("move %s does not exist\n", input);
                continue;
            }

            makemove(move, board);
            white = !white;

            /* Render */
            blitboard(screensurface, board, whitepov);
            renderboard(renderer, screensurface, texture);

            updatecastlestate(move, &castlestate);

            /* Check win */
            end = genallmoves(moves, board, white, true, &castlestate);
            nummoves = end - &(moves[0]);
            if (nummoves == 0) {
                printf("Checkmate, %s wins!\n", white ? "black" : "white");
                exit(0);
            }
            clrstr(input);
        } else if ((isalpha(key) && islower(key)) || isdigit(key)) {
            if (inputp == INPUTSIZE) {
                clrstr(input);
                inputp = 0;
            } else
                input[inputp++] = key;
        } else if (key == SDLK_TAB) {
            whitepov = !whitepov;
            blitboard(screensurface, board, whitepov);
            renderboard(renderer, screensurface, texture);
        }
    } while (!(event.key.keysym.sym == SDLK_ESCAPE));

    SDL_DestroyWindow(window);
    SDL_Quit();
}
