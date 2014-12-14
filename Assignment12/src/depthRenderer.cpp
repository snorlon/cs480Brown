#include <GL/glew.h> // glew must be included before the main gl libs
#include <GL/glut.h> // doing otherwise causes compiler shouting
#include <iostream>
#include <chrono>

//to make compiler happy
#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier



#include "config.h"
#include "renderer.h"
#include "depthRenderer.h"

depthRenderer::depthRenderer()
{
    simConfig = NULL;
    renderModule = NULL;

    depthFrameBuffer = 0;
}

depthRenderer::~depthRenderer()
{

}

void depthRenderer::render()
{
    glBindFramebuffer(GL_FRAMEBUFFER, depthFrameBuffer);
    glViewport(0,0,simConfig->getWindowWidth(),simConfig->getWindowHeight()); // Render on the whole framebuffer, complete from the lower left corner to the upper right

    // We don't use bias in the shader, but instead we draw back faces, 
    // which are already separated from the front faces by a small distance 
    // (if your geometry is made this way)
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use our shader
    simConfig->simShaderManager->activateShader("Depth");


    //force rendering
    simConfig->simEntityManager->depthRender();


    // Render to the screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);//direct at texture image?
    glViewport(0,0,simConfig->getWindowWidth(),simConfig->getWindowHeight()); // Render on the whole framebuffer, complete from the lower left corner to the upper right

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool depthRenderer::initialize()
{
    //Keep here??
    simConfig->simShaderManager->activateShader("Depth");

    // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
    glGenFramebuffers(1, &depthFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, depthFrameBuffer);

    // Depth texture. Slower than a depth buffer, but you can sample it later in your shader
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, 1024, 1024, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

    glDrawBuffer(GL_NONE); // No color buffer is drawn to.

    //additional things
    depthMatrixID = glGetUniformLocation(simConfig->simShaderManager->activeProgram, "depthMVP");

    //Keep here??
    simConfig->simShaderManager->activateShader("PerFragmentLighting");
    TextureID  = glGetUniformLocation(simConfig->simShaderManager->activeProgram, "gSampler");

    DepthBiasID = glGetUniformLocation(simConfig->simShaderManager->activeProgram, "DepthBiasMVP");
    ShadowMapID = glGetUniformLocation(simConfig->simShaderManager->activeProgram, "shadowMap");

    // Always check that our framebuffer is ok
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        cout<<"Failure in depth renderer initialization!"<<endl;
        return false;
    }


    return true;
}
