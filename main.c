#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "globals.h"
#include "rendering.h"
#include "fs.h"

int win_w, win_h;
time_t t_end;
unsigned int slide_delay = 4000;;

unsigned int s_timer_callback(unsigned int interval, void *param)
{
    SDL_Event event;
    SDL_UserEvent userevent;
    /* This callback pushes an SDL_USEREVENT event to change the slide
     * into the queue, and causes our callback to be called again at the
     * same interval: */

    userevent.type = SDL_USEREVENT;
    userevent.code = EVT_SLIDECHANGE;
    userevent.data1 = NULL;
    userevent.data2 = NULL;

    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);
    return(interval);
}
int main(int argc, char **argv) {
    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Texture *images[MAX_SLIDES];
    SDL_TimerID timerid;
    unsigned char quit;
    int curslide=0;
    int nslides;

    if (argc <= 3) {
        printf("Usage: %s <slide_delay_msec> <ttf_font> <image_path>\n", argv[0]);
        return 1;
    }

    /* Compute change delay */
    slide_delay = atoi(argv[1]);
    SDL_Log("Slide change delay set to %d ms", slide_delay);

    /* Initialize font rendering */
    TTF_Init();

    /* Init window */
    if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }

    /* Create rendering context */
    if(SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP, &window, &renderer) < 0 ){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't open SDL window: %s", SDL_GetError());
        return 3;
    }
    if(SDL_GetRendererOutputSize(renderer, &win_w, &win_h) < 0){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't get SDL window size: %s", SDL_GetError());
        return 3;
    }
    
    /* Load the slides/images */
    nslides = load_images(renderer, images, argv[3]);
    if(nslides < 1){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "At least 1 image slide is required");
        return 1;
    }

    /* Prepare for rendering */
    if(RenderingPrepare(&renderer, argv[2]) < 0){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error during final renderer initialization");
        return 3;
    }

    /* Start cycling through slides */
    timerid = SDL_AddTimer(slide_delay, s_timer_callback, (void *)NULL);
    t_end = read_start_time();
    quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&event) == 1) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            } else if(event.type == SDL_USEREVENT && event.user.code == EVT_SLIDECHANGE){	
                // Go to the next slide
                curslide++;
                
                //Check if we are at the end. If so, go back to countdown or slide 1.
                if(curslide > nslides){
                    //If we are about to show the countdown (slide 0), read the time and make sure it is positive.
                    t_end = read_start_time();
                    if(t_end - time(NULL) < 0)
                        curslide=1;
                    else
                        curslide=0;
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        RenderSlide(renderer, curslide, images, quit);

        SDL_RenderPresent(renderer);
	SDL_Delay(100);
    }
    /* cleanup and destroy objects */
    RenderingDestroy();
    free_images(images,nslides);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
