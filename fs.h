#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#include <SDL.h>
#include <SDL_image.h>

int read_start_time(void);
int load_images(SDL_Renderer*, SDL_Texture **, const char *);
void free_images(SDL_Texture **, int);
