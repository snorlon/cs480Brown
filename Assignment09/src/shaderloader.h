#ifndef SHADERLOADERH
#define SHADERLOADERH

#include <GL/glew.h> // glew must be included before the main gl libs
#include <GL/glut.h> // doing otherwise causes compiler shouting
#include <iostream>
#include <chrono>

#include <string.h>  //used for string comparisons for file loading
#include <iostream>
#include <fstream>

//to make compiler happy
#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

class config;
class shaderManager
{
    public:
        char* vertexShader;
        char* fragmentShader;
        char* vertexShader2d;
        char* fragmentShader2d;

        GLuint vertex_shader;
        GLuint fragment_shader;

        GLuint vertex_shader_2d;
        GLuint fragment_shader_2d;
        config* simConfig;

        shaderManager()
        {
            vertexShader = NULL;
            fragmentShader = NULL;
            vertexShader2d = NULL;
            fragmentShader2d = NULL;
        }

        bool giveLinks(config* configData);
        bool loadShaders(int argc, char **argv);
        void activate2DShaders();
        void activate3DShaders();
};

#endif
