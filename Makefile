CC=gcc
CFLAGS=-W -Wall
LDFLAGS=`sdl-config --cflags --libs`
EXEC=tp2

all: $(EXEC)

$(EXEC): tp2main.o tp2Tools.o tp2SDL.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

tp2main.o: tp2main.c tp2.h
	$(CC) -o $@ -c $< $(CFLAGS) $(LDFLAGS)

tp2Tools.o: tp2Tools.c
	$(CC) -o $@ -c $< $(CFLAGS) $(LDFLAGS)

tp2SDL.o: tp2SDL.c
	$(CC) -o $@ -c $< $(CFLAGS) $(LDFLAGS)

clean:
	rm -rf *.o *.txt

mrproper: clean
	rm -rf $(EXEC)
