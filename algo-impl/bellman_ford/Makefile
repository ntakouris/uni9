name = hw2
src = $(wildcard *.cpp) $(wildcard */*.cpp)
obj = $(src:/c=.o)

CC = g++
CFLAGS = -std=c++0x -O3
LIBFLAGS = -lleda -lm

Leda = '/usr/local/LEDA/incl'
LedaLibs = '/usr/local/LEDA'

BOOSTDIR = '/usr/include'

all: $(name)
$(name): $(obj)
	$(CC) $(CFLAGS) -o $@ $^ -I$(Leda) -L$(LedaLibs) $(LIBFLAGS) -I$(BOOSTDIR)

compile: $(name)
$(name): $(obj)
	$(CC) $(CFLAGS) -g -o $@ $^ -I$(Leda) -L$(LedaLibs) $(LIBFLAGS) -I$(BOOSTDIR)

run:
	./$(name)
	
clean:
	rm -f $(name)
