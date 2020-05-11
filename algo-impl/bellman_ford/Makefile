name = hw2
src = $(wildcard *.cpp) $(wildcard */*.cpp)
obj = $(src:/c=.o)

CC = g++
CFLAGS = -std=c++0x -O3
LIBFLAGS = -lleda -lm

Leda = '/usr/local/LEDA/incl'
LedaLibs = '/usr/local/LEDA'

BOOSTINCL = '/usr/include/boost'
BOOSTLIB = '/usr/local/lib'

all: $(name)
$(name): $(obj)
	$(CC) $(CFLAGS) -o $@ $^ -I$(Leda) -L$(LedaLibs) $(LIBFLAGS) -I$(BOOSTINCL) -L$(BOOSTLIB)
	
compile: $(name)
$(name): $(obj)
	$(CC) $(CFLAGS) -o $@ $^ -I$(Leda) -L$(LedaLibs) $(LIBFLAGS) -I$(BOOSTINCL) -L$(BOOSTLIB)

run:
	./$(name)
	
clean:
	rm -f $(name)