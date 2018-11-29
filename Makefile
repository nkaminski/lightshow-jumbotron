CFLAGS += $(shell /opt/rpi-sdl/bin/sdl2-config --cflags) -Wall -O2 -g
LFLAGS += $(shell /opt/rpi-sdl/bin/sdl2-config --libs) -lSDL2 -lSDL2_ttf -lSDL2_image
EXENAME = jumbotron
all:	
	$(CC) main.c rendering.c fs.c $(CFLAGS) $(LFLAGS) -o $(EXENAME)
clean:
	$(RM) $(EXENAME)
