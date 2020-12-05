# lightshow-jumbotron
SDL2 slide show and countdown application used for my holiday light show.

## Requirements
The build scripts for this application expect the following libraries to be built under prefix /opt/rpi-sdl:
* SDL2 (built against rpi videocore driver) - https://www.libsdl.org/
* SDL2_ttf - https://www.libsdl.org/projects/SDL_ttf/
* SDL2_image - https://www.libsdl.org/projects/SDL_image/

## Compiling
Trivial, just run `make`

## Usage
To run this application, invoke such as follows: `./jumbotron <slide_delay_in_millisecondsc> <ttf_font> <image_directory_path>`

If the file `/tmp/nextshow` exists and contains a UNIX timestamp in the future, the "Next show" slide will be dynamically rendered with a countdown ending at the time listed in such file. The included `jumbotron_set_*.sh` scripts contain an example for setting the countdown timer.

## Notes
* Slides will be displayed in alphabetical order based on filename.
* Slides are loaded on startup; add/removal of a slide requires an application restart.
    * `/tmp/nextshow` is an exception to this and is reread every iteration through the set of slides.
