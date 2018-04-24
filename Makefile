CC = gcc
CXX = g++
CFLAGS =  -pedantic -Wall
LDFLAGS = -lglfw3 -lGL -lX11 -lpthread -lXrandr \
		  -lXi -lrt -lm -ldl -lXxf86vm -lGLEW  \
		  -lXinerama -lXcursor -lstdc++

.PHONY: all
all: fractal

fractal: main.o ldsdr.o util.o
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o fractal
