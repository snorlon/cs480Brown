#include "config.h"


config::config()
{
    program = 0;
    
    rotationModifier = 1.0f;

    //--Init the view (camera)
    //  if you will be having a moving camera the view matrix will need to more dynamic
    //  ...Like you should update it before you render more dynamic 
    //  for this project having them static will be fine
    view = glm::lookAt( glm::vec3(0.0, 8.0, -16.0), //Eye Position
                        glm::vec3(0.0, 0.0, 0.0), //Focus point
                        glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up
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

    //assumed success accessing links
    return true;
}

void config::setWindow( int wHeight, int wWidth)
{
    windowHeight = wHeight;
    windowWidth = wWidth;

    //Update the projection matrix when we set the window size
    //See the init function for an explaination
    projection = glm::perspective(45.0f, float(windowWidth)/float(windowHeight), 0.01f, 100.0f);
}
