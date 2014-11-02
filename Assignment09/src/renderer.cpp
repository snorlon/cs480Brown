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

    //activate 3D shader
    simConfig->simShaderManager->activate3DShaders();

    //perform pre-rendering
    glUseProgram(simConfig->program);
    simConfig->simEntityManager->prerender();

    glUseProgram(simConfig->program);

    //throw in entity manager rendering
    simConfig->simEntityManager->render();

    //activate 2D shader
    simConfig->simShaderManager->activate2DShaders();

    //render the 2d images
    sprites.render(  );
                           
    //swap the buffers
    glutSwapBuffers();

    //forces to operate in a finite time
    glFlush();
}

void renderer::tick()
{
    sprites.clearDynamic();

    //BRING IN THE DYNAMICS
    //create our text stuffs
    //sprites.addSprite(simConfig, 300, 200, 85, 232, "testtrainer.png", false);
    sprites.generateText(simConfig, "High Scores", 0.4, 45, 215);
    sprites.generateText(simConfig, "Vs", 0.5, 612, 730);
    sprites.generateText(simConfig, "FPS "+to_string(simConfig->lastFPS), 0.4, 25, 740);
    sprites.generateText(simConfig, "Version 1.0.0", 0.4, 985, 740);
}

bool renderer::initialize()
{
    //enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    sprites.init(simConfig);

    //create static interfaces
    sprites.addSprite(simConfig, 0, 0, 1280, 800, "interface/airhockeyinterface.png", true);

    //and its done
    return true;
}
