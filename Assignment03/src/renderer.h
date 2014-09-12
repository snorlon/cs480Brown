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

class renderer
{
    public:
        config* simConfig;


        renderer();
        bool giveLinks(config* configData);
        void render();
        bool initialize();

    private:
        glm::mat4 model;//obj->world each object should have its own model matrix
        glm::mat4 view;//world->eye
        glm::mat4 projection;//eye->clip
        glm::mat4 mvp;//premultiplied modelviewprojection

        //uniform locations
        GLint loc_mvpmat;// Location of the modelviewprojection matrix in the shader

        //attribute locations
        GLint loc_position;
        GLint loc_color;
};
