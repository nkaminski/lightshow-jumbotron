CFLAGS = -Wall -O2 -g
LIBS = -lSDL2 -lSDL2_ttf
EXENAME = jumbotron
all:	
	$(CC) main.c rendering.c $(CFLAGS) $(LIBS) -o $(EXENAME)
dtest:

	$(CC) drivers.c $(CFLAGS) $(LIBS) -o dtest
clean:
	$(RM) $(EXENAME)
