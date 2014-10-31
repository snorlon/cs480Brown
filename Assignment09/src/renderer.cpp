#include "renderer.h"
#include "config.h"
#include "entity.h"

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
    glClearColor(0.1, 0.1, 0.15, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //perform pre-rendering
    glUseProgram(simConfig->program);
    simConfig->simEntityManager->prerender();

    glUseProgram(simConfig->program);

    //throw in entity manager rendering
    simConfig->simEntityManager->render();

    //render the 2d images
    sprites.render( simConfig );
                           
    //swap the buffers
    glutSwapBuffers();

    //forces to operate in a finite time
    glFlush();
}

bool renderer::initialize()
{
    //enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //do the thing with the sprites
    sprites.load();

    //and its done
    return true;
}
