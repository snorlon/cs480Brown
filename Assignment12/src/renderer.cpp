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
    //--Render the scene
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
    glViewport(0,0,simConfig->getWindowWidth(),simConfig->getWindowHeight());


    glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
    //activate depth shader
    simConfig->simShaderManager->activateShader("Depth");
    GLuint depthProgramID = simConfig->simShaderManager->activeProgram;
    //clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    //throw in entity manager rendering
    simConfig->simEntityManager->render(1);
    //render game objects
    simConfig->gameData.render(1);



    glBindFramebuffer(GL_FRAMEBUFFER, fbo_main);

    //activate 3D shader
    simConfig->simShaderManager->activateShader("PerFragmentLighting");

    //clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

    //throw in entity manager rendering
		
	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0, 
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);

	glm::mat4 depthBiasMVP = biasMatrix*depthMVP;
	glUniformMatrix4fv(depthModule.DepthBiasID, 1, GL_FALSE, &depthBiasMVP[0][0]);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthModule.depthTexture);
    glUniform1i(depthModule.ShadowMapID, 1);

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
    // create the fbo
    glGenFramebuffers(1, &fbo_main);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_main);

    simConfig->simShaderManager->activateShader("2D");

    flatModule.init();

    FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

    depthModule.init();

    //and its done
    return true;
}
