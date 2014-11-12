#ifndef SHADERLOADERH
#define SHADERLOADERH

#include <GL/glew.h> // glew must be included before the main gl libs
#include <GL/glut.h> // doing otherwise causes compiler shouting
#include <iostream>
#include <chrono>

#include <string.h>  //used for string comparisons for file loading
#include <iostream>
#include <fstream>
#include <string>

//to make compiler happy
#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

using namespace std;

class config;
class shaderManager
{
    public:
        config* simConfig;

        //storage for the shaders that are loaded in
        string shaderNames[500];
        GLuint vertexShaders[500];
        GLuint fragmentShaders[500];
        GLuint programs[500];
        int shaderCount;
        int activeShader;
        GLuint activeProgram;

        shaderManager()
        {
            shaderCount = 0;
        }
        ~shaderManager();   

        bool giveLinks(config* configData);
        bool loadShaders();
        void activateShader(string id);
        void activateShader(int id);
        int grabShaderIndex(string shaderName);
};

#endif
