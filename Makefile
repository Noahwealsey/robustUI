CFLAGS = -Wall -Wextra -std=c11 -pedantic -ggdb

s: menu.c
	$(CC) $(CFLAGS) -o s menu.c -lncurses

p: panels.c
	$(CC) $(CFLAGS) -o t panels.c -lncurses -lpanel

