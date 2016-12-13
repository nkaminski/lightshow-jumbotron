#include <stdlib.h>
#include "globals.h"
#include "rendering.h"

static TTF_Font *f_full, *f_half;

/* initializes font rendering */
int font_open(TTF_Font **fp, char* fontpath, int size_divisor){
    *fp = TTF_OpenFont(fontpath, (win_w/CHARS_PER_LINE)/size_divisor);
    if (*fp == NULL) {
        fprintf(stderr, "error: font not found\n");
        return -1;
    }
    return 0;
}
/*
 * - x, y: bottom center point.
 *   - texture, rect: outputs.
 *    Must free texture when done!*/
int get_text_and_rect(SDL_Renderer *renderer, int x, int y, char *text,
        TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect, SDL_Color color) {
    int text_width;
    int text_height;
    SDL_Surface *surface;

    surface = TTF_RenderText_Solid(font, text, color);
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

int RenderingPrepare(SDL_Renderer **ri, char *fontpath){
    if(font_open(&f_full, fontpath, 1) < 0)
        return -1;
    if(font_open(&f_half, fontpath, 2) < 0)
        return -1;
    return 0;
}
int RenderingDestroy(){
    if(f_full != NULL)
        TTF_CloseFont(f_full);
    if(f_half != NULL)
        TTF_CloseFont(f_half);
    return 0;
}
int RenderSlide(SDL_Renderer *r, int slidenum, unsigned char final){
    static SDL_Texture *texture1=NULL;
    static SDL_Rect rect1;
    SDL_Color white = {255,255,255,0};
    
    if(get_text_and_rect(r, win_w/2, win_h, "HELLO WORLD!!", f_full, &texture1, &rect1, white) < 0)
        return -1;
    SDL_RenderCopy(r, texture1, NULL, &rect1);
    SDL_DestroyTexture(texture1);
    /* called to render the final slide, deallocate everything at the end */
    if(final){
    }
    return 0;
}
