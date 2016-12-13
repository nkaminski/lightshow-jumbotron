#include <stdlib.h>

#include "globals.h"
#include "rendering.h"
int win_w, win_h;

int main(int argc, char **argv) {
    SDL_Event event;
    SDL_Rect rect1, rect2;
    SDL_Renderer *renderer;
    SDL_Texture *texture1, *texture2;
    SDL_Window *window;
    char *font_path;
    TTF_Font *font;
    int quit;

    if (argc == 1) {
        font_path = "FreeSans.ttf";
    } else if (argc == 2) {
        font_path = argv[1];
    } else {
        fprintf(stderr, "error: too many arguments\n");
        exit(EXIT_FAILURE);
    }

    /* Init window */
    if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }

    if(SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP, &window, &renderer) < 0 ){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't open SDL window: %s", SDL_GetError());
        return 3;
    }
    if(SDL_GetRendererOutputSize(renderer, &win_w, &win_h) < 0){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't open SDL window: %s", SDL_GetError());
        return 3;
    }
    if (font_init(&font,font_path) < 0) {
        exit(EXIT_FAILURE);
    }
    get_text_and_rect(renderer, win_w/2, win_h, "HELLO WORLD!!", font, &texture1, &rect1);
   // get_text_and_rect(renderer, 0, rect1.y + rect1.h, "world", font, &texture2, &rect2);

    quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&event) == 1) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        /* Use TTF textures. */
        SDL_RenderCopy(renderer, texture1, NULL, &rect1);
        //SDL_RenderCopy(renderer, texture2, NULL, &rect2);

        SDL_RenderPresent(renderer);
    }

    /* Deinit TTF. */
    SDL_DestroyTexture(texture1);
    //SDL_DestroyTexture(texture2);
    font_destroy();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
