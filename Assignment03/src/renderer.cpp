#include "renderer.h"
#include "config.h"

renderer::renderer()
{

}

bool renderer::giveLinks(config* configData)
{
    //abort if any provided links are bogus, we NEED them
    if(configData == NULL)
        return false;

    simConfig = configData;

    //assumed success accessing links
    return true;
}

void renderer::render()
{
    //--Render the scene

    //clear the screen
    glClearColor(0.0, 0.0, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(simConfig->program);

    //throw in entity manager rendering
    simConfig->simEntityManager->render();
                           
    //swap the buffers
    glutSwapBuffers();
}

bool renderer::initialize()
{
    //enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //and its done
    return true;
}
