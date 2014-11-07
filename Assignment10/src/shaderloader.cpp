#include "shaderloader.h"
#include "config.h"
#include <iostream>

using namespace std;

bool shaderManager::giveLinks(config* configData)
{
    //abort if any provided links are bogus, we NEED them
    if(configData == NULL)
        return false;

    simConfig = configData;

    //assumed success accessing links
    return true;
}

bool shaderManager::loadShaders(int argc, char **argv)
{
    //variables
    std::ifstream input;
    int fileLength = 0;

    //first things first, try to load in the shader files if provided
    //check if we have any execution arguements
    if(argc > 1)
    {
        //iterate across all provided parameters
        for(int i = 1; i < argc; i++)
        {
            //check if this parameter is a -v for vertex shader or -f for fragment shader
            if(strcmp(argv[i], "-vv")==0 || strcmp(argv[i], "-fv")==0 || strcmp(argv[i], "-vf")==0 || strcmp(argv[i], "-ff")==0 || strcmp(argv[i], "-2v")==0 || strcmp(argv[i], "-2f")==0)
            {
                //check if we can get the next parameters
                if(i+1<argc)
                {
                    char* tempFilename = new char[strlen(argv[i+1])+1];

                    strcpy(tempFilename, argv[i+1]); //store the file name

                    //load in the vertex shader if filename is good
                    input.clear();
                    input.open(tempFilename);

                    if(input.good())
                    {                        
                        input.seekg(0, std::ios::end);
                        fileLength = input.tellg(); //get file size to size string
                        input.seekg(0, std::ios::beg);
                        if(strcmp(argv[i], "-vv")==0)
                        {
                            vertexShaderPerVertex = new char[fileLength];
                            input.get(vertexShaderPerVertex, fileLength, '\0');
                            std::cout<<"Per Vertex Vertex Shader Loaded!"<<std::endl;
                        }
                        else if(strcmp(argv[i], "-fv")==0)
                        {
                            fragmentShaderPerVertex = new char[fileLength];
                            input.get(fragmentShaderPerVertex, fileLength, '\0');
                            std::cout<<"Per Vertex Fragment Shader Loaded!"<<std::endl;
                        }
                        else if(strcmp(argv[i], "-vf")==0)
                        {
                            vertexShaderPerFragment = new char[fileLength];
                            input.get(vertexShaderPerFragment, fileLength, '\0');
                            std::cout<<"Per Fragment Vertex Shader Loaded!"<<std::endl;
                        }
                        else if(strcmp(argv[i], "-ff")==0)
                        {
                            fragmentShaderPerFragment = new char[fileLength];
                            input.get(fragmentShaderPerFragment, fileLength, '\0');
                            std::cout<<"Per Fragment Fragment Shader Loaded!"<<std::endl;
                        }
                        else if(strcmp(argv[i], "-2v")==0)
                        {
                            vertexShader2d = new char[fileLength];
                            input.get(vertexShader2d, fileLength, '\0');
                            std::cout<<"2D Vertex Shader Loaded!"<<std::endl;
                        }
                        else if(strcmp(argv[i], "-2f")==0)
                        {
                            fragmentShader2d = new char[fileLength];
                            input.get(fragmentShader2d, fileLength, '\0');
                            std::cout<<"2D Fragment Shader Loaded!"<<std::endl;
                        }
                    }
                    input.close();
                }
            }
        }
    }

    //create the shaders
    vertex_shader_v = glCreateShader(GL_VERTEX_SHADER);
    fragment_shader_v = glCreateShader(GL_FRAGMENT_SHADER);
    vertex_shader_f = glCreateShader(GL_VERTEX_SHADER);
    fragment_shader_f = glCreateShader(GL_FRAGMENT_SHADER);
    vertex_shader_2d = glCreateShader(GL_VERTEX_SHADER);
    fragment_shader_2d = glCreateShader(GL_FRAGMENT_SHADER);

    char buffer[512];
    int errLength = 0;

    //compile the shaders
    GLint shader_status;

    // Vertex shader first
    glShaderSource(vertex_shader_v, 1, (const char **)&vertexShaderPerVertex, NULL);

    glCompileShader(vertex_shader_v);
    //check the compile status
    glGetShaderiv(vertex_shader_v, GL_COMPILE_STATUS, &shader_status);

    if(!shader_status)
    {
        glGetShaderInfoLog(vertex_shader_v, 512, &errLength, buffer);
        std::cerr << "[F] FAILED TO COMPILE PER VERTEX VERTEX SHADER!" << std::endl;
        fprintf(stderr, "Compilation error in Vertex Shader: %s\n", buffer);

        return false;
    }

    // Now the Fragment shader
    glShaderSource(fragment_shader_v, 1, (const char **)&fragmentShaderPerVertex, NULL);

    glCompileShader(fragment_shader_v);
    //check the compile status
    glGetShaderiv(fragment_shader_v, GL_COMPILE_STATUS, &shader_status);

    if(!shader_status)
    {
        std::cerr << "[F] FAILED TO COMPILE PER VERTEX FRAGMENT SHADER!" << std::endl;
        glGetShaderInfoLog(fragment_shader_v, 512, &errLength, buffer);
        fprintf(stderr, "Compilation error in Fragment Shader: %s\n", buffer);
    
        return false;
    }

    // Vertex shader first
    glShaderSource(vertex_shader_f, 1, (const char **)&vertexShaderPerFragment, NULL);

    glCompileShader(vertex_shader_f);
    //check the compile status
    glGetShaderiv(vertex_shader_f, GL_COMPILE_STATUS, &shader_status);

    if(!shader_status)
    {
        glGetShaderInfoLog(vertex_shader_f, 512, &errLength, buffer);
        std::cerr << "[F] FAILED TO COMPILE PER FRAG VERTEX SHADER!" << std::endl;
        fprintf(stderr, "Compilation error in Vertex Shader: %s\n", buffer);

        return false;
    }

    // Now the Fragment shader
    glShaderSource(fragment_shader_f, 1, (const char **)&fragmentShaderPerFragment, NULL);

    glCompileShader(fragment_shader_f);
    //check the compile status
    glGetShaderiv(fragment_shader_f, GL_COMPILE_STATUS, &shader_status);

    if(!shader_status)
    {
        std::cerr << "[F] FAILED TO COMPILE PER FRAG FRAGMENT SHADER!" << std::endl;
        glGetShaderInfoLog(fragment_shader_f, 512, &errLength, buffer);
        fprintf(stderr, "Compilation error in Fragment Shader: %s\n", buffer);
    
        return false;
    }


    // 2D Vertex shader
    glShaderSource(vertex_shader_2d, 1, (const char **)&vertexShader2d, NULL);

    glCompileShader(vertex_shader_2d);
    //check the compile status
    glGetShaderiv(vertex_shader_2d, GL_COMPILE_STATUS, &shader_status);

    if(!shader_status)
    {
        glGetShaderInfoLog(vertex_shader_2d, 512, &errLength, buffer);
        std::cerr << "[F] FAILED TO COMPILE 2D VERTEX SHADER!" << std::endl;
        fprintf(stderr, "Compilation error in 2D Vertex Shader: %s\n", buffer);

        return false;
    }

    // 2d Fragment shader
    glShaderSource(fragment_shader_2d, 1, (const char **)&fragmentShader2d, NULL);

    glCompileShader(fragment_shader_2d);
    //check the compile status
    glGetShaderiv(fragment_shader_2d, GL_COMPILE_STATUS, &shader_status);

    if(!shader_status)
    {
        std::cerr << "[F] FAILED TO COMPILE 2D FRAGMENT SHADER!" << std::endl;
        glGetShaderInfoLog(fragment_shader_2d, 512, &errLength, buffer);
        fprintf(stderr, "Compilation error in 2D Fragment Shader: %s\n", buffer);
    
        return false;
    }




    //Now we link the 2 shader objects into a program
    //This program is what is run on the GPU
    simConfig->program = glCreateProgram();
    glAttachShader(simConfig->program, vertex_shader_f);
    glAttachShader(simConfig->program, fragment_shader_f);
    glLinkProgram(simConfig->program);
    glUseProgram(simConfig->program);
    //check if everything linked ok
    glGetProgramiv(simConfig->program, GL_LINK_STATUS, &shader_status);
    if(!shader_status)
    {
        std::cerr << "[F] THE SHADER PROGRAM FAILED TO LINK" << std::endl;
        return false;
    }

    return true;
}

void shaderManager::activate2DShaders()
{
    glDetachShader(simConfig->program, vertex_shader_v);
    glDetachShader(simConfig->program, fragment_shader_v);

    glDetachShader(simConfig->program, vertex_shader_f);
    glDetachShader(simConfig->program, fragment_shader_f);

    glAttachShader(simConfig->program, vertex_shader_2d);
    glAttachShader(simConfig->program, fragment_shader_2d);

    glLinkProgram(simConfig->program);
    glUseProgram(simConfig->program);
}

void shaderManager::activate3DShaders(bool perVertex)
{
    glDetachShader(simConfig->program, vertex_shader_2d);
    glDetachShader(simConfig->program, fragment_shader_2d);

    if(perVertex)
    {
        glDetachShader(simConfig->program, vertex_shader_f);
        glDetachShader(simConfig->program, fragment_shader_f);

        glAttachShader(simConfig->program, vertex_shader_v);
        glAttachShader(simConfig->program, fragment_shader_v);
    }
    else
    {
        glDetachShader(simConfig->program, vertex_shader_v);
        glDetachShader(simConfig->program, fragment_shader_v);

        glAttachShader(simConfig->program, vertex_shader_f);
        glAttachShader(simConfig->program, fragment_shader_f);
    }

    glLinkProgram(simConfig->program);
    glUseProgram(simConfig->program);
}
