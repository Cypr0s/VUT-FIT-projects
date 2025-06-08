# Makefile
# Kristian Luptak
# IJC-DU2, solution a) b)
# 18.4.2025

CC= gcc
CFLAGS= -O2 -g -std=c11 -pedantic -Wall -Wextra 

CXX= g++
CCFLAGS= -std=c++17 -pedantic -Wall

LDFLAGS=

OBJS= 	htab_bucket_count.o \
		htab_clear.o \
		htab_erase.o \
		htab_find.o \
		htab_for_each.o \
		htab_free.o \
		htab_hash_function.o \
		htab_init.o \
		htab_lookup_add.o \
		htab_size.o \

.PHONY: zip all clean check

all: tail libhtab.a libhtab.so maxwordcount-dynamic maxwordcount maxwordcount-cpp

check: all
	./tail -n 10 io.c
	./tail -n 10 io.c | ./maxwordcount
	./tail -n 10 io.c | LD_LIBRARY_PATH=. ./maxwordcount-dynamic


# obj files
%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

# tail
tail: tail.o 
	$(CC) $(CFLAGS) -o $@ $^


#libs
libhtab.a: $(OBJS)
	ar rcs $@ $(OBJS)

libhtab.so: $(OBJS)
	$(CC) $(CFLAGS) -shared -fPIC $(OBJS) -o $@


#maxwordcount/-dynamic
maxwordcount: maxwordcount.o libhtab.a io.o
	$(CC) $(CFLAGS) -o $@ -static $^

maxwordcount-dynamic: maxwordcount.o libhtab.so io.o
	$(CC) $(CFLAGS) -o $@ $^


maxwordcount-cpp: maxwordcount-cpp.cc
	$(CXX) $(CCFLAGS) -o $@ $^



clean:
	rm -rf *.o *.a *.so tail maxwordcount maxwordcount-dynamic maxwordcount-cpp

zip:
	zip xluptak00.zip *.c *.cc *.h Makefile