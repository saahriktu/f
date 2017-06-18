all:
	gcc -o f f.c -lcurses
install:
	cp f /usr/games/
