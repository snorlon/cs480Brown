# Linux
CC=g++
LIBS= -lglut -lGLEW -lGL

# For Macs uncomment the next line and comment out the previous one
#CC=clang++
#LIBS=  -framework OpenGL -framework GLUT -framework Cocoa -lGLEW -stdlib=libc++
# Assuming you want to use a recent compiler

# Compiler flags
CXXFLAGS= -g -Wall -std=c++0x


OUTPUT = ../bin/Project
MAIN = ../src/main.cpp
SHADERLOADER = ../src/shaderloader.cpp
SHADERLOADERH = ../src/shaderloader.h
RENDERER = ../src/renderer.cpp
RENDERERH = ../src/renderer.h
CONFIG = ../src/config.cpp
CONFIGH = ../src/config.h

all: main

main: main.o config.o shaderloader.o renderer.o
	$(CC) $(CXXFLAGS) -o $(OUTPUT) main.o config.o shaderloader.o renderer.o $(LIBS)

main.o: $(MAIN) $(SHADERLOADERH) $(RENDERERH)
	$(CC) $(CXXFLAGS) -c $(MAIN) $(LIBS)

shaderloader.o: $(SHADERLOADER) $(SHADERLOADERH)
	$(CC) $(CXXFLAGS) -c $(SHADERLOADER)

config.o: $(CONFIG) $(CONFIGH)
	$(CC) $(CXXFLAGS) -c $(CONFIG)

renderer.o: $(RENDERER) $(RENDERERH) $(SHADERLOADERH)
	$(CC) $(CXXFLAGS) -c $(RENDERER) $(LIBS)

clean:
	rm ../bin/Project *.o

run:
	./../bin/Project
