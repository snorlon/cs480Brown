# Linux
CC=g++
LIBS= -lglut -lGLEW -lGL -lassimp

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
ENTITYMANAGER = ../src/entitymanager.cpp
ENTITYMANAGERH = ../src/entitymanager.h
ENTITY = ../src/entity.cpp
ENTITYH = ../src/entity.h
CONFIG = ../src/config.cpp
CONFIGH = ../src/config.h

all: main

main: main.o config.o entitymanager.o shaderloader.o renderer.o
	$(CC) $(CXXFLAGS) -o $(OUTPUT) main.o config.o entitymanager.o shaderloader.o renderer.o $(LIBS)

main.o: $(MAIN) $(CONFIGH) $(SHADERLOADERH) $(RENDERERH) $(ENTITYMANAGERH)
	$(CC) $(CXXFLAGS) -c $(MAIN) $(LIBS)

entitymanager.o: $(ENTITYMANAGER) $(ENTITYMANAGERH) $(CONFIGH) $(ENTITYH)
	$(CC) $(CXXFLAGS) -c $(ENTITYMANAGER)

entity.o: $(ENTITY) $(ENTITYH) $(CONFIGH) $(LIBS)
	$(CC) $(CXXFLAGS) -c $(ENTITY)

shaderloader.o: $(SHADERLOADER) $(SHADERLOADERH) $(CONFIGH)
	$(CC) $(CXXFLAGS) -c $(SHADERLOADER)

config.o: $(CONFIG) $(CONFIGH) $(ENTITYMANAGERH) $(SHADERLOADERH) $(LIBS)
	$(CC) $(CXXFLAGS) -c $(CONFIG)

renderer.o: $(RENDERER) $(RENDERERH) $(SHADERLOADERH) $(CONFIGH)
	$(CC) $(CXXFLAGS) -c $(RENDERER) $(LIBS)

clean:
	rm ../bin/Project *.o

run:
	./../bin/Project
