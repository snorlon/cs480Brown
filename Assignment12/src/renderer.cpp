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
    //--Render the scene
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glViewport(0,0,simConfig->getWindowWidth(),simConfig->getWindowHeight());

    simConfig->simShaderManager->activateShader("Depth");

    //lights render
    simConfig->worldLights->render();

    //upload models
    simConfig->simEntityManager->uploadVertices();

    //switch buffer
    glBindFramebuffer(GL_FRAMEBUFFER, flatRenderModule.fbo_drawn);

    //draw them
    simConfig->simEntityManager->drawVertices();

    //finish up with the shadow mapping
    glViewport(0,0,simConfig->getWindowWidth(),simConfig->getWindowHeight());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


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

    simConfig->simShaderManager->activateShader("Depth");

    glGetUniformLocation(programID, "depthMVP");

    simConfig->simShaderManager->activateShader("PerFragmentLighting");
    DepthBiasID = glGetUniformLocation(programID, "DepthBiasMVP");

    simConfig->simShaderManager->activateShader("PerFragmentLighting");
    ShadowMapID = glGetUniformLocation(programID, "shadowMap");


	flatRenderModule.initialize();
	depthRenderModule.initialize();

	// The depth buffer
	if ( !GLEW_ARB_framebuffer_object ){ // OpenGL 2.1 doesn't require this, 3.1+ does
		printf("Your GPU does not provide framebuffer objects. Use a texture instead.");
		return false;
	}
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, simConfig->getWindowWidth(), simConfig->getWindowHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTexture, 0);

	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

    //enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //create static interfaces
    flatRenderModule.spriteModule.addSprite(simConfig, 0, 0, 1280, 800, "interface/airhockeyinterface.png", true);


    //prep the depth lighting
    glGenFramebuffers(1, &fbo_depth);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_depth);

     glGenTextures(1, &depthTexture);
     glBindTexture(GL_TEXTURE_2D, depthTexture);
     glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, 1024, 1024, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
     
     glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
     
     glDrawBuffer(GL_NONE); // No color buffer is drawn to.
     
     // Always check that our framebuffer is ok
     if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cout<<"DOES NOT WORK"<<endl;

    //and its done
    return true;
}
