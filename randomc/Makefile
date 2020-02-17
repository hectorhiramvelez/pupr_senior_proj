# Compiler Options
CC=g++
CFLAGS=-Wall -Wextra -O2


# Object files
OBJS=mersenne.o mother.o sfmt.o userintf.o


# Final Configuration
COMPILE=$(CC) $(CFLAGS)


# MAKE Configuration
.DEFAULT_GOAL := librandomc.a
.PHONY := all clean install


# Phony targets
all: librandomc.a ex-ran
clean:
	rm -rf librandomc.a ex-ran $(OBJS)

install: librandomc.a
	echo "TODO: make install"


# Main targets
librandomc.a: $(OBJS)
	ar -cq librandomc.a $(OBJS)

ex-ran: ex-ran.o $(OBJS)
	$(COMPILE) -o ex-ran ex-ran.o $(OBJS)

testirandomx: testirandomx.o $(OBJS)
	$(COMPILE) -o testirandomx testirandomx.o $(OBJS)


# Main Object Files
ex-ran.o: ex-ran.cpp randomc.h
	$(COMPILE) -c -o ex-ran.o ex-ran.cpp

testirandomx.o: testirandomx.cpp sfmt.h randomc.h
	$(COMPILE) -c -o testirandomx.o testirandomx.cpp


# Object files
mersenne.o: mersenne.cpp randomc.h
	$(COMPILE) -c -o mersenne.o mersenne.cpp

mother.o: mother.cpp randomc.h
	$(COMPILE) -c -o mother.o mother.cpp

sfmt.o: sfmt.cpp sfmt.h randomc.h
	$(COMPILE) -c -o sfmt.o sfmt.cpp

userintf.o: userintf.cpp
	$(COMPILE) -c -o userintf.o userintf.cpp
