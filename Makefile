CPP = g++
CPPFLAGS = -O2

# Using clang++ on Mac OS for better speed
uname := $(shell sh -c 'uname -s 2>/dev/null || echo not')
ifeq ($(uname),Darwin)
	CPP = clang++
	CPPFLAGS = -O3
endif

OBJECTS = main.o lodepng.o

all: $(OBJECTS)
	$(CPP) $(OBJECTS) $(CPPFLAGS) -o rain

main.o:
	$(CPP) main.cpp $(CPPFLAGS) -c

lodepng.o:
	$(CPP) lodepng.cpp $(CPPFLAGS) -c

clean:
	rm -rf rain *.o

