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

class shaderManager
{
    public:
        char* vertexShader;
        char* fragmentShader;

        shaderManager()
        {
            vertexShader = NULL;
            fragmentShader = NULL;
        }

    void loadShaders(int argc, char **argv);
};

