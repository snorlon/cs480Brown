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

    flatRenderModule.simConfig = configData;
    flatRenderModule.renderModule = this;

    depthRenderModule.simConfig = configData;
    depthRenderModule.renderModule = this;

    //assumed success accessing links
    return true;
}

void renderer::render()
{
    depthRenderModule.render();

    //switch buffer
    glBindFramebuffer(GL_FRAMEBUFFER, flatRenderModule.fbo_drawn);

    /*simConfig->simShaderManager->activateShader("Depth");

    //lights render
    simConfig->worldLights->render();

    //upload models
    simConfig->simEntityManager->uploadVertices();

    //draw them
    simConfig->simEntityManager->drawVertices();*/


    //activate 3D shader
    simConfig->simShaderManager->activateShader("PerFragmentLighting");

    //clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    //throw in entity manager rendering
    simConfig->simEntityManager->render();

    //render game objects
    simConfig->gameData.render();


    //Will the real frame buffer please stand up
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    //glClearColor(0.5, 0.5, 0.9, 1.0);
    glViewport(0,0,simConfig->getWindowWidth(),simConfig->getWindowHeight());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glBindBuffer(GL_ARRAY_BUFFER, outputSprite->vbo_sprite);
    flatRenderModule.render();

    //unbind buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
                           
    //swap the buffers
    glutSwapBuffers();

    //forces to operate in a finite time
    glFlush();
}

void renderer::tick()
{
    flatRenderModule.spriteModule.clearDynamic();

    //BRING IN THE DYNAMICS
    //create our text stuffs
    //sprites.addSprite(simConfig, 300, 200, 85, 232, "testtrainer.png", false);
    flatRenderModule.spriteModule.generateText(simConfig, "High Scores", 0.4, 45, 215);
    flatRenderModule.spriteModule.generateText(simConfig, to_string(int(ceil(simConfig->gameData.time))), 0.5, 612, 730);
    flatRenderModule.spriteModule.generateText(simConfig, "FPS "+to_string(simConfig->lastFPS), 0.4, 25, 740);
    flatRenderModule.spriteModule.generateText(simConfig, "Version 1.0.0", 0.4, 985, 740);

    //scoreboard stuff
    flatRenderModule.spriteModule.generateText(simConfig, ""+to_string(int(floor(simConfig->gameData.currentGame.currScore))), 0.5, 425, 740);

    //render GAMEOVER if game is not active
    if(simConfig->gameData.gameState==2)
        flatRenderModule.spriteModule.generateText(simConfig, "YOU WIN!", 1.0, 410, 440, 30);
    else if(simConfig->gameData.gameState==1)
        flatRenderModule.spriteModule.generateText(simConfig, "YOU LOSE!", 1.0, 410, 440, 30);

    //draw highscores
    for(int i=0; i<10; i++)
    {
        if(simConfig->gameData.highscores[i]!=NULL)
        {
            score* currentScore = simConfig->gameData.highscores[i];//for readability
            flatRenderModule.spriteModule.generateText(simConfig, to_string(int(floor(currentScore->currScore))), 0.3, 225, 180-20*i,-2);//draw score
        }
    }
}

bool renderer::initialize()
{
    GLuint programID = simConfig->simShaderManager->activeProgram;

	flatRenderModule.initialize();
	depthRenderModule.initialize();

    //enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //create static interfaces
    flatRenderModule.spriteModule.addSprite(simConfig, 0, 0, 1280, 800, "interface/airhockeyinterface.png", true);
     
     // Always check that our framebuffer is ok
     if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cout<<"DOES NOT WORK"<<endl;

    //and its done
    return true;
}
