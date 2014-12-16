#ifndef FLATRENDERERH
#define FLATRENDERERH
#include <GL/glew.h> // glew must be included before the main gl libs
#include <GL/glut.h> // doing otherwise causes compiler shouting
#include <iostream>
#include <chrono>

//to make compiler happy
#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier
#include "sprite.h"

class renderer;
class config;
class flatrenderer
{
    public:
        config* simConfig;
        renderer* simRenderer;


        
        spriteManager sprites;//holds all 2d image data

        GLuint renderedTexture;
        GLuint depthrenderbuffer;
        GLuint texID;

        sprite* outputSprite;





        flatrenderer();
        ~flatrenderer();

        void init();
        void tick();
        void render();

    private:
};

#endif


