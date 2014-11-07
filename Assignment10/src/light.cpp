#include "light.h"

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

void lightSource::on()
{
    if(lights==NULL)
        return;

    stateOn = true;

    //set all of our values in the master light array
    //set the position
    lights->position[id*4 + 0] = position[0];//x
    lights->position[id*4 + 1] = position[1];//y
    lights->position[id*4 + 2] = position[2];//z
    lights->position[id*4 + 3] = position[3];//??

    //set the ambient
    lights->sourceAmbient[id*4 + 0] = sourceAmbient[0];//R
    lights->sourceAmbient[id*4 + 1] = sourceAmbient[1];//G
    lights->sourceAmbient[id*4 + 2] = sourceAmbient[2];//B
    lights->sourceAmbient[id*4 + 3] = sourceAmbient[3];//A

    //set the diffuse
    lights->sourceDiffuse[id*4 + 0] = sourceDiffuse[0];//R
    lights->sourceDiffuse[id*4 + 1] = sourceDiffuse[1];//G
    lights->sourceDiffuse[id*4 + 2] = sourceDiffuse[2];//B
    lights->sourceDiffuse[id*4 + 3] = sourceDiffuse[3];//A

    //set the specular
    lights->sourceSpecular[id*4 + 0] = sourceSpecular[0];//R
    lights->sourceSpecular[id*4 + 1] = sourceSpecular[1];//G
    lights->sourceSpecular[id*4 + 2] = sourceSpecular[2];//B
    lights->sourceSpecular[id*4 + 3] = sourceSpecular[3];//A
}

void lightSource::off()
{
    stateOn = false;

    //set all of our values in the master light array
    //set the position
    lights->position[id*4 + 0] = 0;//x
    lights->position[id*4 + 1] = 0;//y
    lights->position[id*4 + 2] = 0;//z
    lights->position[id*4 + 3] = 0;//??

    //set the ambient
    lights->sourceAmbient[id*4 + 0] = 0;//R
    lights->sourceAmbient[id*4 + 1] = 0;//G
    lights->sourceAmbient[id*4 + 2] = 0;//B
    lights->sourceAmbient[id*4 + 3] = 0;//A

    //set the diffuse
    lights->sourceDiffuse[id*4 + 0] = 0;//R
    lights->sourceDiffuse[id*4 + 1] = 0;//G
    lights->sourceDiffuse[id*4 + 2] = 0;//B
    lights->sourceDiffuse[id*4 + 3] = 0;//A

    //set the specular
    lights->sourceSpecular[id*4 + 0] = 0;//R
    lights->sourceSpecular[id*4 + 1] = 0;//G
    lights->sourceSpecular[id*4 + 2] = 0;//B
    lights->sourceSpecular[id*4 + 3] = 0;//A
}

lightArray::lightArray()
{
    head = NULL;

    position = NULL;
    sourceAmbient = NULL;
    sourceDiffuse = NULL;
    sourceSpecular = NULL;

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

void lightArray::init()
{
    //start the arrays off with +1 in case we have 0 lights, just in case, you know?
    position = new double[(lightCount+1)*4];
    sourceAmbient = new double[(lightCount+1)*4];
    sourceDiffuse = new double[(lightCount+1)*4];
    sourceSpecular = new double[(lightCount+1)*4];

    //default them to 0
    for(int i=0; i< (lightCount+1)*4; i++)
    {
        position[i] = 0.0;
        sourceAmbient[i] = 0.0;
        sourceDiffuse[i] = 0.0;
        sourceSpecular[i] = 0.0;
    }

    //flip them switches
    on();
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
