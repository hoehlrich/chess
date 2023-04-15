#include "chess.h" 
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>

#define WIDTH   512
#define HEIGHT  512
#define BLIT(src) SDL_BlitScaled(pieces, &src, surface, &dstpiece)

SDL_Rect newrect(int x, int y, int w, int h);

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
    SDL_Rect atlas[50];

    atlas[42] = newrect(0, 0, 320, 320);        /* k */
    atlas[48] = newrect(320, 0, 320, 320);      /* q */
    atlas[33] = newrect(640, 0, 320, 320);      /* b */
    atlas[45] = newrect(960, 0, 320, 320);      /* n */
    atlas[49] = newrect(1280, 0, 320, 320);     /* r */
    atlas[47] = newrect(1600, 0, 320, 320);     /* p */
    atlas[10] = newrect(0, 320, 320, 320);      /* K */
    atlas[16] = newrect(320, 320, 320, 320);    /* Q */
    atlas[1]  = newrect(640, 320, 320, 320);    /* B */
    atlas[13] = newrect(960, 320, 320, 320);    /* N */
    atlas[17] = newrect(1280, 320, 320, 320);   /* R */
    atlas[15] = newrect(1600, 320, 320, 320);   /* P */

    SDL_Rect king = { 256, 448, 64, 64 };
    for (y = 0; y < BLEN; y++) {
        for (x = 0; x < BLEN; x++) {
            SDL_FillRect(surface, &square, (y + x) % 2 ? dark : light);
            if (isalpha(board[y][x]))
                SDL_BlitScaled(pieces, &atlas[board[y][x]-65], surface, &dstpiece);

            square.x += WIDTH/BLEN;
            dstpiece.x += WIDTH/BLEN;
        }
        square.y += HEIGHT/BLEN;
        square.x = 0;

        dstpiece.y += HEIGHT/BLEN;
        dstpiece.x = 0;
    }
}

SDL_Rect newrect(int x, int y, int w, int h) {
    SDL_Rect rect = { x, y, w, h };
    return rect;
}
