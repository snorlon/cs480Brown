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

    flatModule.simConfig = simConfig;
    depthModule.simConfig = simConfig;
    flatModule.simRenderer = this;
    depthModule.simRenderer = this;

    //assumed success accessing links
    return true;
}

void renderer::render()
{
    // create the fbo
    glGenFramebuffers(1, &fbo_main);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_main);

    //--Render the scene
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_main);
    glViewport(0,0,simConfig->getWindowWidth(),simConfig->getWindowHeight());

    //activate 3D shader
    simConfig->simShaderManager->activateShader("PerFragmentLighting");

    //clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //throw in entity manager rendering
    simConfig->simEntityManager->render();

    //render game objects
    simConfig->gameData.render();


    flatModule.render();
                           
    //swap the buffers
    glutSwapBuffers();

    //forces to operate in a finite time
    glFlush();
}

void renderer::tick()
{
    flatModule.tick();
}

bool renderer::initialize()
{
    simConfig->simShaderManager->activateShader("2D");

    flatModule.init();

    //and its done
    return true;
}
