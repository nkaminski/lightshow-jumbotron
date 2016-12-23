#include <stdlib.h>
#include <time.h>
#include "globals.h"
#include "rendering.h"

static TTF_Font *f_full, *f_half;
//TESTING ONLY
const time_t t_end = 1481614841;

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
int RenderSlide(SDL_Renderer *r, int *slidenum, unsigned char final){
    static SDL_Texture *texture_l1=NULL, *texture_l2=NULL;
    static SDL_Rect rect_l1, rect_l2;
    SDL_Color white = {255,255,255,0};
    SDL_Color red = {255,58,58,0};
    SDL_Color green = {58,255,50,0};
    char timebuf[256];
    char advance=1;
    time_t t_current = time(NULL);
    time_t t_delta = t_end - t_current;
    /* SLIDE SPECIFIC RENDERING CODE HERE */
    while(advance){
        advance=0;
        switch(*slidenum){
            case 0:
                /* timer slide */
                if(t_current >= t_end){
                    //skip slide
                    (*slidenum)++;
                    advance=1;
                    break;
                }
                snprintf(timebuf, 256, "%02ld:%02ld", t_delta/60, t_delta % 60);
                if(get_text_and_rect(r, win_w/2, win_h/6, 1, "Next Show:", f_half, &texture_l1, &rect_l1, white) < 0)
                    return -1;
                SDL_RenderCopy(r, texture_l1, NULL, &rect_l1);
                if(get_text_and_rect(r, win_w/2, (win_h/6)+TTF_FontLineSkip(f_half), 1, timebuf, f_full, &texture_l2, &rect_l2, white) < 0)
                    return -1;
                SDL_RenderCopy(r, texture_l2, NULL, &rect_l2);
                SDL_DestroyTexture(texture_l1);
                texture_l1=NULL;
                SDL_DestroyTexture(texture_l2);
                texture_l2=NULL;

                break;
            case 1:
                if(get_text_and_rect(r, win_w/2, win_h/6, 1, "Tune Radio To:", f_half, &texture_l1, &rect_l1, white) < 0)
                    return -1;
                SDL_RenderCopy(r, texture_l1, NULL, &rect_l1);
                if(get_text_and_rect(r, win_w/2, (win_h/6)+TTF_FontLineSkip(f_half), 1, "89.9 FM", f_full, &texture_l2, &rect_l2, white) < 0)
                    return -1;
                SDL_RenderCopy(r, texture_l2, NULL, &rect_l2);
                SDL_DestroyTexture(texture_l1);
                texture_l1=NULL;
                SDL_DestroyTexture(texture_l2);
                texture_l2=NULL;

                break;
            case 2:
                if(get_text_and_rect(r, win_w/2, win_h/3, 1, "Happy", f_half, &texture_l1, &rect_l1, red) < 0)
                    return -1;
                SDL_RenderCopy(r, texture_l1, NULL, &rect_l1);
                if(get_text_and_rect(r, win_w/2, (win_h/3)+TTF_FontLineSkip(f_half), 1, "Holidays!", f_half, &texture_l2, &rect_l2, green) < 0)
                    return -1;
                SDL_RenderCopy(r, texture_l2, NULL, &rect_l2);
                SDL_DestroyTexture(texture_l1);
                texture_l1=NULL;
                SDL_DestroyTexture(texture_l2);
                texture_l2=NULL;
                break;

            default:
                *slidenum = 0;
                advance=1;
                break;
        }
    }
    /* called to render the final slide, deallocate everything at the end */
    if(final){
        if(texture_l1 != NULL)
            SDL_DestroyTexture(texture_l1);
        if(texture_l2 != NULL)
            SDL_DestroyTexture(texture_l2);
    }
    return 0;
}
