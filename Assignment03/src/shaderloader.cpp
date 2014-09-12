#include "shaderloader.h"
#include "config.h"

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
            if(strcmp(argv[i], "-v")==0 || strcmp(argv[i], "-f")==0)
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
                        if(strcmp(argv[i], "-v")==0)
                        {
                            vertexShader = new char[fileLength];
                            input.get(vertexShader, fileLength, '\0');
                            std::cout<<"Vertex Shader Loaded!"<<std::endl;
                        }
                        else
                        {
                            fragmentShader = new char[fileLength];
                            input.get(fragmentShader, fileLength, '\0');
                            std::cout<<"Fragment Shader Loaded!"<<std::endl;
                        }
                    }
                    input.close();
                }
            }
        }
    }

    //attempt to use some default shaders if none are given to us
    if(vertexShader == NULL)
    {
        vertexShader = new char[300];
        strcpy( vertexShader, 
        "attribute vec3 v_position;"
        "attribute vec3 v_color;"
        "varying vec3 color;"
        "uniform mat4 mvpMatrix;"
        "void main(void){"
        "   gl_Position = mvpMatrix * vec4(v_position, 1.0);"
        "   color = v_color;"
        "}");
    }

    if(fragmentShader == NULL)
    {
        fragmentShader = new char[100];
        strcpy( fragmentShader, 
        "varying vec3 color;"
        "void main(void){"
        "   gl_FragColor = vec4(color.rgb, 1.0);"
        "}");
    }

    //create the shaders
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    //compile the shaders
    GLint shader_status;

    // Vertex shader first
    glShaderSource(vertex_shader, 1, (const char **)&vertexShader, NULL);

    glCompileShader(vertex_shader);
    //check the compile status
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &shader_status);
    if(!shader_status)
    {
        std::cerr << "[F] FAILED TO COMPILE VERTEX SHADER!" << std::endl;
        return false;
    }

    // Now the Fragment shader
    glShaderSource(fragment_shader, 1, (const char **)&fragmentShader, NULL);

    glCompileShader(fragment_shader);
    //check the compile status
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &shader_status);
    if(!shader_status)
    {
        std::cerr << "[F] FAILED TO COMPILE FRAGMENT SHADER!" << std::endl;
        return false;
    }

    //Now we link the 2 shader objects into a program
    //This program is what is run on the GPU
    simConfig->program = glCreateProgram();
    glAttachShader(simConfig->program, vertex_shader);
    glAttachShader(simConfig->program, fragment_shader);
    glLinkProgram(simConfig->program);
    //check if everything linked ok
    glGetProgramiv(simConfig->program, GL_LINK_STATUS, &shader_status);
    if(!shader_status)
    {
        std::cerr << "[F] THE SHADER PROGRAM FAILED TO LINK" << std::endl;
        return false;
    }

    return true;
}
