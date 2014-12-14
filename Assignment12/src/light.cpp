#include "light.h"
#include "config.h"
#include "renderer.h"

#include <GL/glew.h> // glew must be included before the main gl libs
#include <GL/glut.h> // doing otherwise causes compiler shouting

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

lightSource::lightSource( lightArray* l, entity* p )
{
    parent = p;
    next = NULL;

    id = l->lightCount;
    l->lightCount++;

    if(l->head == NULL)
    {
        l->head = this;
    }
    else
    {
        lightSource* iterator = l->head;
        while(iterator->next != NULL)
            iterator = iterator->next;

        iterator->next = this;
    }

    lights = l;

    type = POINTLIGHT;

    stateOn = false;//default to off

}

lightSource::~lightSource()
{

}

void lightSource::setPosition(double values[4])
{
    position[0] = values[0];
    position[1] = values[1];
    position[2] = values[2];
    position[3] = values[3];
}

void lightSource::setAmbient(double values[4])
{
    sourceAmbient[0] = values[0];
    sourceAmbient[1] = values[1];
    sourceAmbient[2] = values[2];
    sourceAmbient[3] = values[3];
}

void lightSource::setDiffuse(double values[4])
{
    sourceDiffuse[0] = values[0];
    sourceDiffuse[1] = values[1];
    sourceDiffuse[2] = values[2];
    sourceDiffuse[3] = values[3];
}

void lightSource::setSpecular(double values[4])
{
    sourceSpecular[0] = values[0];
    sourceSpecular[1] = values[1];
    sourceSpecular[2] = values[2];
    sourceSpecular[3] = values[3];
}

void lightSource::setSpotlight(double values[4], double val2)
{
    sourceSpotlightDirection[0] = values[0];
    sourceSpotlightDirection[1] = values[1];
    sourceSpotlightDirection[2] = values[2];
    sourceSpotlightDirection[3] = values[3];

    sourceSpotlightCutoff = val2;
}

void lightSource::on()
{
    if(lights==NULL)
        return;

    stateOn = true;
}

void lightSource::off()
{
    stateOn = false;
}

void lightSource::render()
{
    GLuint depthMatrixID = lights->simConfig->simRenderer->depthMatrixID;

    //do nothing
    glm::vec3 lightInvDir = glm::vec3(0.5f,2,2);

    // Compute the MVP matrix from the light's point of view
    glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10,10,-10,10,-10,20);
    glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0,0,0), glm::vec3(0,1,0));
    glm::mat4 depthModelMatrix = glm::mat4(1.0);
    glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, &depthMVP[0][0]);
}

lightArray::lightArray()
{
    head = NULL;

    simConfig = NULL;

    lightCount = 0;
}
lightArray::~lightArray()
{
    while(head!=NULL)
    {
        lightSource* temp = head;
        head = head->next;
        delete temp;
    }
}

bool lightArray::addLight(double pos[4], double ambient[4], double diffuse[4], double specular[4], double spotdir[4], double spotcut, entity* parent)
{
    lightSource* newLight = NULL;
    if(lightCount>=10)
        return false;

    newLight = new lightSource(this, parent);

    newLight->setPosition(pos);
    newLight->setAmbient(ambient);
    newLight->setDiffuse(diffuse);
    newLight->setSpecular(specular);
    newLight->setSpotlight(spotdir, spotcut);

    return true;
}

void lightArray::init()
{
}

void lightArray::on()
{
    //TOGGLE THEM LIGHTS LIKE THE BADASS YOU ARE, FLIP THEM SWITCHES LIKE YOU JUST DON'T CARE
    lightSource* iterator = head;
    while(iterator!=NULL)
    {
        iterator->on();
        iterator = iterator->next;
    }
}

void lightArray::off()
{
    //TOGGLE THEM LIGHTS LIKE THE BADASS YOU ARE, FLIP THEM SWITCHES LIKE YOU JUST DON'T CARE
    lightSource* iterator = head;
    while(iterator!=NULL)
    {
        iterator->off();
        iterator = iterator->next;
    }
}

void lightArray::render()
{
    simConfig->simShaderManager->activateShader("Depth");


    lightSource* iterator = head;
    while(iterator!=NULL)
    {
        iterator->render();
        iterator = iterator->next;
    }
}

entityLightNode::entityLightNode( lightSource* newL)
{
    node = newL;
    next = NULL;
}
entityLightNode::~entityLightNode()
{
    //do nothing,not our job to cleanup
}


entityLight::entityLight()
{
    lightNodes = NULL;//no lights attached to start

    shine = 30;
}

entityLight::~entityLight()
{
    while(lightNodes!=NULL)
    {
        entityLightNode* temp = lightNodes;
        lightNodes = lightNodes->next;
        delete temp;
    }
}

void entityLight::on()
{
    //TOGGLE THEM LIGHTS LIKE THE BADASS YOU ARE, FLIP THEM SWITCHES LIKE YOU JUST DON'T CARE
    entityLightNode* iterator = lightNodes;
    while(iterator!=NULL)
    {
        iterator = iterator->next;
        iterator->node->on();
    }
}

void entityLight::off()
{
    //TOGGLE THEM LIGHTS LIKE THE BADASS YOU ARE, FLIP THEM SWITCHES LIKE YOU JUST DON'T CARE
    entityLightNode* iterator = lightNodes;
    while(iterator!=NULL)
    {
        iterator = iterator->next;
        iterator->node->off();
    }
}
