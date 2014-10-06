#!/bin/bash
make clean
make
./../bin/Project -f ../bin/shaders/fragmentshader -v ../bin/shaders/vertexshader -m ../bin/models/towerbase.obj
