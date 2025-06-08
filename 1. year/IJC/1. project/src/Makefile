# Makefile
# Author	Kristian Luptak
# Date 		22.3.2025
# Solution	IJC-DU1, priklad a), b)

CC=gcc
CFLAGS= -O2 -std=c11 -pedantic -Wall -Wextra
DFLAGS= -g
LDFLAGS= -lm

INLINE=-DUSE_INLINE

.PHONY: run zip all clean 

all: primes primes-i steg-decode

run: primes primes-i
	@ulimit -s 100000 && ./primes && ./primes-i

# error

error.o: error.c error.h
	$(CC) $(CFLAGS) -o $@ -c $< $(DFLAGS)

# primes

primes.o: primes.c
	$(CC) $(CFLAGS) -o $@ -c $< $(DFLAGS)

eratosthenes.o: eratosthenes.c eratosthenes.h
	$(CC) $(CFLAGS) -o $@ -c $< $(DFLAGS)

primes-i.o: primes.c
	$(CC) $(INLINE) $(CFLAGS) -o $@ -c $< $(DFLAGS)

eratosthenes-i.o: eratosthenes.c eratosthenes.h
	$(CC) $(INLINE) $(CFLAGS) -o $@ -c $< $(DFLAGS)

primes: primes.o eratosthenes.o error.o
	$(CC) $(CFLAGS) -o $@ $^ $(DFLAGS) $(LDFLAGS)

primes-i: primes-i.o eratosthenes-i.o error.o
	$(CC) $(INLINE) $(CFLAGS) -o $@ $^ $(DFLAGS) $(LDFLAGS)

# steg-decode

ppm.o: ppm.c ppm.h
	$(CC) $(CFLAGS) -o $@ -c $< $(DFLAGS)

steg-decode.o: steg-decode.c
	$(CC) $(CFLAGS) -o $@ -c $< $(DFLAGS)

steg-decode: ppm.o steg-decode.o eratosthenes.o error.o
	$(CC) $(CFLAGS) -o $@ $^ $(DFLAGS) $(LDFLAGS)


zip:
	zip xluptak00.zip *.c *.h Makefile

clean:
	rm -f *.o primes primes-i steg-decode