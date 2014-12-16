#include "depthrenderer.h"
#include "config.h"
#include "renderer.h"

depthrenderer::depthrenderer()
{
    simConfig = NULL;
    simRenderer = NULL;
}

depthrenderer::~depthrenderer()
{

}

void depthrenderer::init()
{
    //activate depth shader
    simConfig->simShaderManager->activateShader("Depth");

    GLuint depthProgramID = simConfig->simShaderManager->activeProgram;

	// Get a handle for our buffers
	depth_vertexPosition_modelspaceID = glGetAttribLocation(depthProgramID, "vertexPosition_modelspace");

    glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, 1024, 1024, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
		 
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

	// No color output in the bound framebuffer, only depth.
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// Always check that our framebuffer is ok
	glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return;

	
	/*static const GLfloat g_quad_vertex_buffer_data[] = { 
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
	};*/

    /*glGenBuffers(1, &quad_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);*/

    //activate 3D shader
    simConfig->simShaderManager->activateShader("PerFragmentLighting");

    programID = simConfig->simShaderManager->activeProgram;

    DepthBiasID = glGetUniformLocation(programID, "DepthBiasMVP");
	ShadowMapID = glGetUniformLocation(programID, "shadowMap");
    
}

void depthrenderer::render()
{
}
