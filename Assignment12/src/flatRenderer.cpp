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
#include "flatRenderer.h"
#include "sprite.h"

flatRenderer::flatRenderer()
{
    simConfig = NULL;
    renderModule = NULL;
}

flatRenderer::~flatRenderer()
{

}

void flatRenderer::render()
{
    //activate 2D shader
    simConfig->simShaderManager->activateShader("2D");


    //render the 2d images
    //spriteModule.render(  );


    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderedTexture);
    // Set our "renderedTexture" sampler to user Texture Unit 0
    glUniform1i(texID, 0);


    glBindBuffer(GL_ARRAY_BUFFER, flatRenderSprite->vbo_sprite);

    //set up the Vertex Buffer Object so it can be drawn
    glEnableVertexAttribArray(flatRenderSprite->loc_position);

    glVertexAttribPointer( flatRenderSprite->loc_position,//location of attribute
                           3,//number of elements
                           GL_FLOAT,//type
                           GL_FALSE,//normalized?
                           sizeof(Vertex),//stride
                           0);//offset

    glEnableVertexAttribArray(flatRenderSprite->loc_texture);
    glVertexAttribPointer( flatRenderSprite->loc_texture,
                           2,
                           GL_FLOAT,
                           GL_FALSE,
                           sizeof(Vertex),
                            (void*)offsetof(Vertex,uv));

    glDrawArrays(GL_QUADS, 0, 4);//mode, starting index, count

    //clean up
    glDisableVertexAttribArray(flatRenderSprite->loc_position);
    glDisableVertexAttribArray(flatRenderSprite->loc_texture);
}

bool flatRenderer::initialize()
{
    simConfig->simShaderManager->activateShader("2D");

    GLuint programID = simConfig->simShaderManager->activeProgram;

    // create the fbo
    glGenFramebuffers(1, &fbo_drawn);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_drawn);

	texID = glGetUniformLocation(programID, "gSampler");



    // The texture we're going to render to
	glGenTextures(1, &renderedTexture);
	
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, renderedTexture);

	// Give an empty image to OpenGL ( the last "0" means "empty" )
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, simConfig->getWindowWidth(), simConfig->getWindowHeight(), 0,GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

    
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, simConfig->getWindowWidth(), simConfig->getWindowHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTexture, 0);


    //create a sprite to render
    flatRenderSprite = new sprite(0, 0, simConfig->getWindowWidth(), simConfig->getWindowHeight(), 1.0, 1.0);
    flatRenderSprite->load(simConfig,"interface/airhockeyinterface.png");
    flatRenderSprite->init(simConfig);

    //enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    spriteModule.init(simConfig);

    //create static interfaces
    spriteModule.addSprite(simConfig, 0, 0, 1280, 800, "interface/airhockeyinterface.png", true);


    return true;//just give it what it wants, please!
}
