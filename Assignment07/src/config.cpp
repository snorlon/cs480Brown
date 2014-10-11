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

    presetCameras = NULL;

    currentCamera = -1;

    currentFocalCamera = &targetCamera;

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
    eyeCamera.Position(1,1,-2);

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


Camera* config::switchCamera(int camID)
{
    //abort if we don't have any preset cameras
    if(presetCameras == NULL)
        return NULL;

    Camera* iterator = presetCameras;

    while(camID>1 && iterator!=NULL)
    {
        if(iterator->next!=NULL)
        {
            iterator = iterator->next;
        }
        camID--;
    }

    view = glm::lookAt( glm::vec3(eyeCamera.x, eyeCamera.y, eyeCamera.z), //Eye Position
                        glm::vec3(iterator->x, iterator->y, iterator->z), //camera aim
                        glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

    currentFocalCamera = iterator;

    return iterator;
}

void config::tick(float dt)
{
    eyeCamera.tick(dt);
    targetCamera.tick(dt);

    Camera* iterator = presetCameras;
    while(iterator!=NULL)
    {
        iterator->tick(dt);

        iterator = iterator->next;
    }


    if(currentFocalCamera!=NULL)
        view = glm::lookAt( glm::vec3(eyeCamera.x, eyeCamera.y, eyeCamera.z), glm::vec3(currentFocalCamera->x,currentFocalCamera->y,currentFocalCamera->z), glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

}

void config::setWindow( int wHeight, int wWidth)
{
    windowHeight = wHeight;
    windowWidth = wWidth;

    //Update the projection matrix when we set the window size
    //See the init function for an explaination
    projection = glm::perspective(45.0f, float(windowWidth)/float(windowHeight), 0.01f, 100.0f);
}
