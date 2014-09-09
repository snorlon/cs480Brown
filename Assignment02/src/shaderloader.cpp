#include "shaderloader.h"

void shaderManager::loadShaders(int argc, char **argv)
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
}
