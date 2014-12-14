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
}

depthRenderer::~depthRenderer()
{

}

void depthRenderer::render()
{

}

bool depthRenderer::initialize()
{

    return true;
}
