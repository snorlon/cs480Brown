#ifndef DEPTHRENDERERH
#define DEPTHRENDERERH
#include <GL/glew.h> // glew must be included before the main gl libs
#include <GL/glut.h> // doing otherwise causes compiler shouting
#include <iostream>
#include <chrono>

//to make compiler happy
#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

class renderer;
class config;
class depthrenderer
{
    public:
        config* simConfig;
        renderer* simRenderer;

        GLuint depthTexture;

        GLuint programID;

        GLuint DepthBiasID;
	    GLuint ShadowMapID;

        //GLuint quad_vertexbuffer;

        // Get a handle for our buffers
        GLuint depth_vertexPosition_modelspaceID;

        depthrenderer();
        ~depthrenderer();

        void init();
        void render();

    private:
};

#endif


