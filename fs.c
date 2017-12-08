#include "fs.h"
int load_images(SDL_Renderer* renderer, SDL_Texture **textures, const char *texturedir){
    int count=0;

    struct dirent **namelist;
    int i,n;
    size_t name_len;

    //Declare path buffer and make sure it is always NULL terminated
    char path_buffer[PATH_MAX + 1];
    path_buffer[PATH_MAX] = 0x00;

    //List the texture directory
    n = scandir(texturedir, &namelist, 0, alphasort);
    if (n < 0){
        perror("scandir failed while loading images");
        return count;
    }
    else {
        for (i = 0; i < n; i++) {
            name_len = strlen(namelist[i]->d_name);
            // Skip . and ..
            if(name_len != 0 && namelist[i]->d_name[name_len-1] != '.'){

                SDL_Log("Load image: %s as slide %d\n", namelist[i]->d_name, count);

                // Build full path
                snprintf(path_buffer,PATH_MAX,"%s/%s",texturedir,namelist[i]->d_name);

                //Try to load the image
                textures[count] = IMG_LoadTexture(renderer, path_buffer);
                if(textures[count] == NULL){
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image\n");
                }
                else{
                    count++;
                } 
            }
            free(namelist[i]);
        }
        free(namelist);
    }
    return count;
}

void free_images(SDL_Texture **textures, int count){
    int i;
    for(i=0;i<count;i++){
        SDL_DestroyTexture(textures[i]);
    }
}

int read_start_time(){
    FILE* fileptr;
    char timestr[32];
    /* opening file for reading end time*/
    int t_end = 0;
    fileptr = fopen("/tmp/nextshow" , "r");
    if( fileptr != NULL && fgets(timestr, 32, fileptr) != NULL ) 
    {
        t_end = atoi(timestr);
    }
    if(fileptr)
        fclose(fileptr);
    return t_end;
}
