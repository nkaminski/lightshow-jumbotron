#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

int get_text_and_rect(SDL_Renderer *renderer, int x, int y, char *text,
        TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect);
int font_init(TTF_Font **fp, char* fontpath);
void font_destroy();
