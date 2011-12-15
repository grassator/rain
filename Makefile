CPP = g++
CPPFLAGS = -O2

# Using clang++ on Mac OS for better speed
uname := $(shell sh -c 'uname -s 2>/dev/null || echo not')
ifeq ($(uname),Darwin)
	CPP = clang++
	CPPFLAGS = -O3
endif

OBJECTS = main.o lodepng.o guidelines.o

all: rain

rain: $(OBJECTS)
	$(CPP) $(OBJECTS) $(CPPFLAGS) -o rain

%.o: %.cpp Makefile
	$(CPP) $(@:.o=.cpp) $(CPPFLAGS) -c

guidelines.cpp: guidelines.h

clean:
	rm -f rain *.o

