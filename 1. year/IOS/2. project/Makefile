# Makefile
# Kristian Luptak
# IOS project 2
# 25.4.2025

CC= gcc
CFLAGS= -std=gnu99 -Wall -Wextra -Werror -pedantic

.PHONY: zip all clean

all: proj2

# proj2
proj2: proj2.o
	$(CC) $(CFLAGS) -o $@ $^

# obj file
proj2.o: proj2.c
	$(CC) $(CFLAGS) -o $@ -c $<

#clean
clean:
	rm -f *.o proj2

#zip
zip:
	zip xluptak00.zip *.c *.h Makefile