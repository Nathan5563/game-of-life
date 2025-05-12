all: gol

gol: main.c
	gcc main.c -lSDL2 -o gol

clean:
	rm -f gol