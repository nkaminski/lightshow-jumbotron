CFLAGS = -Wall -O2 -g
LIBS = -lSDL2 -lSDL2_ttf -lSDL2_image
EXENAME = jumbotron
all:	
	$(CC) main.c rendering.c fs.c $(CFLAGS) $(LIBS) -o $(EXENAME)
dtest:

	$(CC) drivers.c $(CFLAGS) $(LIBS) -o dtest
clean:
	$(RM) $(EXENAME)
