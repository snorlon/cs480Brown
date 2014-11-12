#include "shaderloader.h"
#include "config.h"
#include <iostream>
#include <sstream>

using namespace std;

shaderManager::~shaderManager()
{
    for(int i=0;i<shaderCount;i++)
        glDeleteProgram( programs[i] );
}

bool shaderManager::giveLinks(config* configData)
{
    //abort if any provided links are bogus, we NEED them
    if(configData == NULL)
        return false;

    simConfig = configData;

    //assumed success accessing links
    return true;
}

bool shaderManager::loadShaders()
{
    //variables
    ifstream input;
    string tempString;
    GLint shader_status;

    //open the shader file and load in all of the shaders
    input.open("../bin/shaders/shader_list");
    input>>tempString;

    while(input.good() && shaderCount<500)
    {
        //attempt to add the shader
        ifstream vertexLoad;
        ifstream fragmentLoad;
        string vertexLoaded;
        string fragmentLoaded;

        vertexLoad.open("../bin/shaders/"+tempString+"/vertexshader");
        fragmentLoad.open("../bin/shaders/"+tempString+"/fragmentshader");

        if(vertexLoad.good() && fragmentLoad.good())
        {
            stringstream sbuffer;
            char buffer[512];
            int errLength = 0;
            bool good = true;

            sbuffer << vertexLoad.rdbuf();
            vertexLoaded = sbuffer.str();

            sbuffer.str("");
            sbuffer << fragmentLoad.rdbuf();
            fragmentLoaded = sbuffer.str();

            //atempt to compile it here because idgaf
            vertexShaders[shaderCount] = glCreateShader(GL_VERTEX_SHADER);
            fragmentShaders[shaderCount] = glCreateShader(GL_FRAGMENT_SHADER);

            const char* vp = vertexLoaded.c_str();
            const char** vstring = &vp;


            // Vertex shader first
            glShaderSource(vertexShaders[shaderCount], 1, vstring, NULL);
            glCompileShader(vertexShaders[shaderCount]);

            //check the compile status
            glGetShaderiv(vertexShaders[shaderCount], GL_COMPILE_STATUS, &shader_status);

            if(!shader_status)
            {
                glGetShaderInfoLog(vertexShaders[shaderCount], 512, &errLength, buffer);
                std::cerr << "[F] FAILED TO COMPILE "+tempString+" VERTEX SHADER!" << std::endl;

                good = false;
            }

            const char* fp = fragmentLoaded.c_str();
            const char** fstring = &fp;

            // frag shader
            glShaderSource(fragmentShaders[shaderCount], 1, fstring, NULL);
            glCompileShader(fragmentShaders[shaderCount]);

            //check the compile status
            glGetShaderiv(fragmentShaders[shaderCount], GL_COMPILE_STATUS, &shader_status);

            if(!shader_status)
            {
                glGetShaderInfoLog(fragmentShaders[shaderCount], 512, &errLength, buffer);
                std::cerr << "[F] FAILED TO COMPILE "+tempString+" FRAGMENT SHADER!" << std::endl;

                good = false;
            }

            if(good)
            {

                //use the first set of shaders, existing or not
                //Now we link the 2 shader objects into a program
                //This program is what is run on the GPU
                programs[shaderCount] = glCreateProgram();
                glAttachShader(programs[shaderCount], vertexShaders[shaderCount]);
                glAttachShader(programs[shaderCount], fragmentShaders[shaderCount]);
                glLinkProgram(programs[shaderCount]);
                //check if everything linked ok
                glGetProgramiv(programs[shaderCount], GL_LINK_STATUS, &shader_status);

                if(shader_status)
                {
                    shaderNames[shaderCount] = tempString;
                    cout<<tempString+" shader loaded successfully!"<<endl;
                    shaderCount++;
                }
            }
        }

        input>>tempString;
    }

    input.close();

    activateShader(0);

    return true;
}

void shaderManager::activateShader(string id)
{
    int shaderIndex = grabShaderIndex(id);

    if(shaderIndex == -1)
    {
        cout<<"Invalid shader "+id+"!"<<endl;
    }
    glUseProgram(programs[shaderIndex]);

    activeShader = shaderIndex;
    activeProgram = programs[shaderIndex];
}

void shaderManager::activateShader(int id)
{
    int shaderIndex = id;

    if(shaderIndex < 0 || shaderIndex >= shaderCount)
    {
        cout<<"Invalid shader "<<id<<"!"<<endl;
    }
    glUseProgram(programs[shaderIndex]);

    activeShader = shaderIndex;
    activeProgram = programs[shaderIndex];
}

int shaderManager::grabShaderIndex(string shaderName)
{
    int i = 0;
    while(i<shaderCount)
    {
        if(shaderNames[i] == shaderName)
            return i;

        i++;
    }

    return -1;
}
