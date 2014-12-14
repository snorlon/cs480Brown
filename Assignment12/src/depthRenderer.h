#ifndef DEPTHRENDERH
#define DEPTHRENDERH

#include <GL/glew.h> // glew must be included before the main gl libs
#include <GL/glut.h> // doing otherwise causes compiler shouting
#include <iostream>
#include <chrono>

//to make compiler happy
#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

class config;
class renderer;
class depthRenderer
{
    public:
        config* simConfig;
        renderer* renderModule;

        GLuint depthFrameBuffer;

        GLuint depthTexture;
        GLuint depthMatrixID;//for the depth matrix

        GLuint TextureID;

        GLuint DepthBiasID;
        GLuint ShadowMapID;

        depthRenderer();
        ~depthRenderer();
        void render();
        bool initialize();

    private:
};


#endif
