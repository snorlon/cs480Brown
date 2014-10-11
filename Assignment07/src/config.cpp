#include "config.h"
#include <iostream>
#include <fstream>

config::config()
{
    program = 0;
    
    objectCount = 1;
    texCount = 0;

    scale = 1.0;

    ifstream input;
    string data;
    input.open("../bin/data/config");
    int line = 0;

    input>>data;

    while(input.good())
    {
        switch(line)
        {
            case 0:
                scale = atof(data.c_str());
                break;
        }

        input>>data;
    }

    input.close();
    

    //--Init the view (camera)
    //  if you will be having a moving camera the view matrix will need to more dynamic
    //  ...Like you should update it before you render more dynamic 
    //  for this project having them static will be fine
    eyeCamera.Position(2,2,-4);

    //update the camera
    recalcCamera();
}


bool config::giveLinks(shaderManager* shaderMgr, entityManager* entMgr)
{
    //abort if any provided links are bogus, we NEED them
    if(shaderMgr == NULL)
        return false;
    if(entMgr == NULL)
        return false;

    simShaderManager = shaderMgr;
    simEntityManager = entMgr;

    std::cout<<"Config loaded!"<<std::endl;

    //assumed success accessing links
    return true;
}

void config::recalcCamera()
{
    view = glm::lookAt( glm::vec3(eyeCamera.x, eyeCamera.y, eyeCamera.z), //Eye Position
                        glm::vec3(targetCamera.x, targetCamera.y, targetCamera.z), //Focus point
                        glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up
}

void config::setWindow( int wHeight, int wWidth)
{
    windowHeight = wHeight;
    windowWidth = wWidth;

    //Update the projection matrix when we set the window size
    //See the init function for an explaination
    projection = glm::perspective(45.0f, float(windowWidth)/float(windowHeight), 0.01f, 100.0f);
}
