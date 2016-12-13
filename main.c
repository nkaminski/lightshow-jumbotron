#include <stdlib.h>

#include "globals.h"
#include "rendering.h"
int win_w, win_h;

int main(int argc, char **argv) {
    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;
    char *font_path;
    unsigned char quit;

    if (argc == 1) {
        font_path = "FreeSans.ttf";
    } else if (argc == 2) {
        font_path = argv[1];
    } else {
        fprintf(stderr, "error: too many arguments\n");
        exit(EXIT_FAILURE);
    }
    TTF_Init();
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
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't get SDL window size: %s", SDL_GetError());
        return 3;
    }
    if(RenderingPrepare(&renderer, font_path) < 0){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error during final renderer initialization");
        return 3;
    }

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
        RenderSlide(renderer, 0, quit);

        SDL_RenderPresent(renderer);
    }
    /* cleanup and destroy objects */
    RenderingDestroy();
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
