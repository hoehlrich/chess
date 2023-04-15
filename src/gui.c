#include "chess.h" 
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>

#define WIDTH   512
#define HEIGHT  512
#define BLIT(src) SDL_BlitScaled(pieces, &src, surface, &dstpiece)

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Surface *screensurface;

/* init: do necessary SDL init stuff */
int init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());


    window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    if (window == NULL)
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());

    renderer = SDL_CreateRenderer(window, -1, 0);

    screensurface = SDL_GetWindowSurface(window);


    return 0;
}

/* renderboard: render the game board and pieces given the board pointer */
void renderboard(SDL_Surface *surface, int (*board)[BLEN]) {
    Uint32 dark = SDL_MapRGB(surface->format, 130, 76, 26);
    Uint32 light = SDL_MapRGB(surface->format, 226, 194, 97);
    int y, x;
    SDL_Rect square = { 0, 0, WIDTH/BLEN, HEIGHT/BLEN };
    SDL_Rect dstpiece = { 0, 0, 64, 64 };
    SDL_Surface *pieces = IMG_Load("assets/pieces.svg.png");

    SDL_Rect k = { 0, 0, 320, 320 };
    SDL_Rect q = { 320, 0, 320, 320 };
    SDL_Rect b = { 640, 0, 320, 320 };
    SDL_Rect n = { 960, 0, 320, 320 };
    SDL_Rect r = { 1280, 0, 320, 320 };
    SDL_Rect p = { 1600, 0, 320, 320 };
    SDL_Rect K = { 0, 320, 320, 320 };
    SDL_Rect Q = { 320, 320, 320, 320 };
    SDL_Rect B = { 640, 320, 320, 320 };
    SDL_Rect N = { 960, 320, 320, 320 };
    SDL_Rect R = { 1280, 320, 320, 320 };
    SDL_Rect P = { 1600, 320, 320, 320 };

    for (y = 0; y < BLEN; y++) {
        for (x = 0; x < BLEN; x++) {
            SDL_FillRect(surface, &square, (y + x) % 2 ? dark : light);
            switch (board[y][x]) {
                case 'k':
                    BLIT(k);
                    break;
                case 'q':
                    BLIT(q);
                    break;
                case 'r':
                    BLIT(r);
                    break;
                case 'b':
                    BLIT(b);
                    break;
                case 'n':
                    BLIT(n);
                    break;
                case 'p':
                    BLIT(p);
                    break;
                case 'K':
                    BLIT(K);
                    break;
                case 'Q':
                    BLIT(Q);
                    break;
                case 'R':
                    BLIT(R);
                    break;
                case 'B':
                    BLIT(B);
                    break;
                case 'N':
                    BLIT(N);
                    break;
                case 'P':
                    BLIT(P);
                    break;
            }
            square.x += WIDTH/BLEN;
            dstpiece.x += WIDTH/BLEN;
        }
        square.y += HEIGHT/BLEN;
        square.x = 0;

        dstpiece.y += HEIGHT/BLEN;
        dstpiece.x = 0;
    }
}

