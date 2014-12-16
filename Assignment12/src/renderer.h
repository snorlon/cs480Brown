#include <GL/glew.h> // glew must be included before the main gl libs
#include <GL/glut.h> // doing otherwise causes compiler shouting
#include <iostream>
#include <chrono>

//to make compiler happy
#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

#include "config.h"
#include "shaderloader.h"
#include "sprite.h"
#include "depthrenderer.h"
#include "flatrenderer.h"

class renderer
{
    public:
        flatrenderer flatModule;
        depthrenderer depthModule;

        GLuint fbo_main;//render to texture
        GLuint FramebufferName;//render to shadow map, replace with fbo_main?
        glm::mat4 depthMVP;

        config* simConfig;

        renderer();
        bool giveLinks(config* configData);
        void render();
        void tick();
        bool initialize();

    private:
};
