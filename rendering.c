#include <stdlib.h>
#include <time.h>
#include "globals.h"
#include "rendering.h"

static TTF_Font *f_full, *f_half;

/* initializes font rendering */
int font_open(TTF_Font **fp, char* fontpath, int size_divisor){
    *fp = TTF_OpenFont(fontpath, (win_w/TEXT_SIZE_DIVISOR)/size_divisor);
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
int get_text_and_rect(SDL_Renderer *renderer, int x, int y, int refloc, char *text,
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
    if(refloc == 1){
        //top
        rect->y = y;
    } 
    else if(refloc == 0){
        //center
        rect->y = y-(text_height/2);
    }
    else{
        //bottom
        rect->y = y-text_height;
    }
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
int RenderSlide(SDL_Renderer *r, int slidenum, SDL_Texture **slides, unsigned char final){
    static SDL_Texture *texture_l1=NULL, *texture_l2=NULL;
    static SDL_Rect rect_l1, rect_l2;
//    SDL_Color white = {255,255,255,0};
    SDL_Color red = {255,58,58,0};
    SDL_Color green = {58,255,50,0};
    char timebuf[256];
    time_t t_delta = t_end - time(NULL);
    /* SLIDE SPECIFIC RENDERING CODE HERE */
    if(slidenum == 0){
                /* timer slide */
                if(t_delta < 0)
                    t_delta=0;
                
                // Does the first line need rendering?
                if(!texture_l1){
                    if(get_text_and_rect(r, win_w/2, win_h/6, 1, "Next Show:", f_half, &texture_l1, &rect_l1, red) < 0)
                        return -1;
                }
                
                //Render the 2nd line
                snprintf(timebuf, 256, "%2ld:%02ld", t_delta/60, t_delta % 60);
                SDL_RenderCopy(r, texture_l1, NULL, &rect_l1);
                if(get_text_and_rect(r, win_w/2, (win_h/6)+TTF_FontLineSkip(f_half), 1, timebuf, f_full, &texture_l2, &rect_l2, green) < 0)
                    return -1;
                SDL_RenderCopy(r, texture_l2, NULL, &rect_l2);
                SDL_DestroyTexture(texture_l2);
                texture_l2=NULL;
    } else {
        //Render an image slide
        SDL_RenderCopy(r, slides[slidenum-1], NULL, NULL); 
    }
    /* called to render the final iteration, deallocate everything at the end */
    if(final){
        if(texture_l1 != NULL)
            SDL_DestroyTexture(texture_l1);
        if(texture_l2 != NULL)
            SDL_DestroyTexture(texture_l2);
    }
    return 0;
}
