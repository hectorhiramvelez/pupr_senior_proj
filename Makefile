# Compiler configuration
CC=g++
GCC=gcc
#CFLAGS=-Wall -Wextra -O0 -pthread -g
CFLAGS=-Wall -Wextra -O2 -pthread
#CFLAGS=-Wall -Wextra -O3 -ffast-math -pthread -fgcse -fgcse-lm -fgcse-sm -fgcse-las -fgcse-after-reload


# Object files
OBJS=autolock.o charge.o charge_c.o charge_s.o grid.o menu.o progress.o prompt.o ring.o rings_thread.o string.o system.o vector2.o vector3.o


# Libraries
BOOST_THREAD=-lboost_thread
RANDOMC=-L./randomc -lrandomc -I./randomc
LIBS=$(RANDOMC) $(BOOST_THREAD)
STATIC=$(RANDOMC)


# Final Configuration
COMPILE=$(CC) $(CFLAGS) $(LIBS)
GCC_COMPILE=$(GCC) $(CFLAGS) $(LIBS)


# Make Configuration
.DEFAULT_GOAL:= main
.PHONY:= all clean debug install run run-test force


# Phony Targets
all: main test
clean:
	rm -f main main.o test test.o $(OBJS)
	$(MAKE) -C randomc clean

debug: main
	gdb ./main

install: main
	echo "TODO: make install"

objects: main.o test.o $(OBJS)

run: main
	./main

run-test: test
	./test

force: ;


# Main targets
main: main.o $(OBJS) randomc/librandomc.a
	$(COMPILE) -o main main.o $(OBJS) $(STATIC)

test: test.o $(OBJS) randomc/librandomc.a
	$(COMPILE) -o test test.o $(OBJS) $(STATIC)


# Main objects
main.o: main.cpp main.hpp grid.hpp ring.hpp charge.hpp charge_c.hpp charge_s.hpp vector3.hpp
	$(COMPILE) -c -o main.o main.cpp

test.o: test.cpp grid.hpp ring.hpp prompt.hpp prompt.h
	$(COMPILE) -c -o test.o test.cpp


# Object files
autolock.o: autolock.cpp autolock.hpp
	$(COMPILE) -c -o autolock.o autolock.cpp

charge.o: charge.cpp charge.hpp vector3.hpp
	$(COMPILE) -c -o charge.o charge.cpp

charge_c.o: charge_c.cpp charge_c.hpp charge_s.hpp charge.hpp vector3.hpp
	$(COMPILE) -c -o charge_c.o charge_c.cpp

charge_s.o: charge_s.cpp charge_s.hpp charge.hpp vector3.hpp
	$(COMPILE) -c -o charge_s.o charge_s.cpp

grid.o: grid.cpp grid.hpp charge_c.hpp charge_s.hpp charge.hpp vector3.hpp
	$(COMPILE) -c -o grid.o grid.cpp

menu.o: menu.hpp menu.cpp prompt.hpp string.hpp
	$(COMPILE) -c -o menu.o menu.cpp

progress.o: progress.cpp progress.hpp autolock.hpp
	$(COMPILE) -c -o progress.o progress.cpp

prompt.o: prompt.cpp prompt.hpp string.hpp
	$(COMPILE) -c -o prompt.o prompt.cpp

ring.o: ring.cpp ring.hpp charge.hpp vector3.hpp
	$(COMPILE) -c -o ring.o ring.cpp

rings_thread.o: rings_thread.cpp rings_thread.hpp ring.hpp charge.hpp vector3.hpp
	$(COMPILE) -c -o rings_thread.o rings_thread.cpp

string.o: string.cpp string.hpp
	$(COMPILE) -c -o string.o string.cpp

system.o: system.cpp system.hpp
	$(COMPILE) -c -o system.o system.cpp

vector2.o: vector2.cpp vector2.hpp
	$(COMPILE) -c -o vector2.o vector2.cpp

vector3.o: vector2.cpp vector2.hpp
	$(COMPILE) -c -o vector3.o vector3.cpp


# Libraries
randomc/librandomc.a: force
	$(MAKE) -C randomc librandomc.a
