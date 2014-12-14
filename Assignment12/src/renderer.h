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
#include "flatRenderer.h"
#include "depthRenderer.h"

class renderer
{
    public:
        config* simConfig;

        GLuint depthMatrixID;//for the depth matrix
        GLuint DepthBiasID;
        GLuint ShadowMapID;

        GLuint fbo_depth;
        GLuint depthTexture;
        GLuint depthrenderbuffer;

        //rendering modules
        flatRenderer flatRenderModule;
        depthRenderer depthRenderModule;

        renderer();
        bool giveLinks(config* configData);
        void render();
        void tick();
        bool initialize();

    private:
};
