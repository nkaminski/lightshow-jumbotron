#include <stdlib.h>
#include "globals.h"
#include "rendering.h"

/* initializes font rendering */
int font_init(TTF_Font **fp, char* fontpath){
    TTF_Init();
    *fp = TTF_OpenFont(fontpath, win_w/CHARS_PER_LINE);
    if (*fp == NULL) {
        fprintf(stderr, "error: font not found\n");
        return -1;
    }
    return 0;
}
void font_destroy(){
    TTF_Quit();
}
/*
 * - x, y: bottom center point.
 *   - texture, rect: outputs.
 *    Must free texture when done!*/
int get_text_and_rect(SDL_Renderer *renderer, int x, int y, char *text,
        TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect) {
    int text_width;
    int text_height;
    SDL_Surface *surface;
    SDL_Color textColor = {255, 255, 255, 0};

    surface = TTF_RenderText_Solid(font, text, textColor);
    if(surface == NULL)
        return -1;

    *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if(*texture == NULL) 
        return -1;

    text_width = surface->w;
    text_height = surface->h;
    SDL_FreeSurface(surface);
    rect->x = x-(text_width/2);
    rect->y = y-text_height;
    rect->w = text_width;
    rect->h = text_height;
    return 0;
}

