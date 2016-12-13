#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

int get_text_and_rect(SDL_Renderer *renderer, int x, int y, int refloc, char *text,
        TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect, SDL_Color color);
int font_open(TTF_Font **fp, char* fontpath, int size_divisor);
int RenderingPrepare(SDL_Renderer **r, char *fontpath);
int RenderSlide(SDL_Renderer *r, int *slidenum, unsigned char final);
int RenderingDestroy();
