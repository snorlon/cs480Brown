# Linux
CC=g++
LIBS= -lglut -lGLEW -lGL -lfreeimageplus -lassimp

# For Macs uncomment the next line and comment out the previous one
#CC=clang++
#LIBS=  -framework OpenGL -framework GLUT -framework Cocoa -lGLEW -stdlib=libc++
# Assuming you want to use a recent compiler

# Compiler flags
CXXFLAGS= -g -Wall -Wextra -std=c++0x $(DEFS)


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
CAMERA = ../src/camera.cpp
CAMERAH = ../src/camera.h

all: main

main: main.o config.o entitymanager.o shaderloader.o camera.o renderer.o
	$(CC) $(CXXFLAGS) -o $(OUTPUT) main.o config.o entitymanager.o shaderloader.o camera.o renderer.o $(LIBS)

main.o: $(MAIN) $(CONFIGH) $(SHADERLOADERH) $(RENDERERH) $(ENTITYH) $(ENTITYMANAGERH)
	$(CC) $(CXXFLAGS) -c $(MAIN) $(LIBS)

entitymanager.o: $(ENTITYMANAGER) $(ENTITYMANAGERH) $(CONFIGH) $(ENTITYH)
	$(CC) $(CXXFLAGS) -c $(ENTITYMANAGER) $(LIBS)

entity.o: $(ENTITY) $(ENTITYH) $(CONFIGH) $(LIBS)
	$(CC) $(CXXFLAGS) -c $(ENTITY)

shaderloader.o: $(SHADERLOADER) $(SHADERLOADERH) $(CONFIGH)
	$(CC) $(CXXFLAGS) -c $(SHADERLOADER)

config.o: $(CONFIG) $(CONFIGH) $(ENTITYMANAGERH) $(SHADERLOADERH) $(CAMERAH) $(LIBS)
	$(CC) $(CXXFLAGS) -c $(CONFIG)

renderer.o: $(RENDERER) $(RENDERERH) $(SHADERLOADERH) $(ENTITYH) $(CONFIGH)
	$(CC) $(CXXFLAGS) -c $(RENDERER) $(LIBS)

camera.o: $(CAMERA) $(CAMERAH)
	$(CC) $(CXXFLAGS) -c $(CAMERA)

clean:
	rm ../bin/Project *.o
