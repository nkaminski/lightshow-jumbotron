#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "globals.h"
#include "rendering.h"
int win_w, win_h;
time_t t_end;
unsigned int slide_delay = SLIDEDELAY_MS;

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
    SDL_TimerID timerid;
    FILE *fileptr;
    char *font_path;
    char timestr[32];
    unsigned char quit;
    int curslide=0;

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
    /* Start cycling through slides */
    timerid = SDL_AddTimer(SLIDEDELAY_MS, s_timer_callback, (void *)NULL);

    quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&event) == 1) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            } else if(event.type == SDL_USEREVENT && event.user.code == EVT_SLIDECHANGE){	

		    /* opening file for reading end time*/
		    fileptr = fopen("/tmp/nextshow" , "r");
		    if(fileptr != NULL){
		    	if( fgets (timestr, 32, fileptr)!=NULL ) 
		    	{
			    t_end = atoi(timestr);
		    	}
		    	fclose(fileptr);
	    	    } else {
			t_end = 0;
		   }
                curslide++;
                if(curslide >= NUM_SLIDES)
                    curslide=0;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        RenderSlide(renderer, &curslide, quit);

        SDL_RenderPresent(renderer);
	usleep(1E5);
    }
    /* cleanup and destroy objects */
    RenderingDestroy();
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
