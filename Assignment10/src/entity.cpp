#include "entity.h"
#include <cstddef>

#include <GL/glew.h> // glew must be included before the main gl libs
#include <GL/glut.h> // doing otherwise causes compiler shouting

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <vector>
#include "shaderloader.h"

using namespace std;

#define AU 149597871

entity::entity(config* nConfig) //load a model from a file
{
    srand (0);//set random for now to preset
    next = NULL;
    simConfig = nConfig;

    //give the config to our physics ASAP, physics gotta phys
    objPhysics.simConfig = simConfig;

    texWidth = 0;
    texHeight = 0;

    texID = 0;

    visible = true;

    name = "Error";

    id = simConfig->objectCount;
    simConfig->objectCount++;

    //children = NULL;
    parent = NULL;

    scale = 1.0;

    size.x = 1.0;
    size.y = 1.0;
    size.z = 1.0;

    orientation.x = 0.0;
    orientation.y = 0.0;
    orientation.z = 0.0;

    glGenBuffers(1, &vbo_geometry);
}

entity::~entity()
{
    glDeleteBuffers(1, &vbo_geometry);
    glDeleteTextures(1, &texID);
}

void entity::cleanup()
{
    glDeleteBuffers(1, &vbo_geometry);

    /*entity* iterator = children;

    while(iterator!=NULL)
    {
        iterator->cleanup();

        iterator = iterator->next;
    }*/
}

void entity::init()
{
    simConfig->simShaderManager->activate3DShaders(simConfig->lightPerVertex);
    //save how much stuff is in the buffer for future rendering purposes
    vertexCount = vertices.size();
    entity* me = this;

    objPhysics.init( me );//init our bullet physics
    
    // Create a Vertex Buffer object to store this vertex info on the GPU
    glBindBuffer(GL_ARRAY_BUFFER, vbo_geometry);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    //Now we set the locations of the attributes and uniforms
    //this allows us to access them easily while rendering
    loc_position = glGetAttribLocation(simConfig->program, "v_position");
    if(loc_position == -1)
    {
        std::cerr << "[F] VPOSITION NOT FOUND" << std::endl;
    }

    loc_normal = glGetAttribLocation(simConfig->program, "v_normal");
    if(loc_position == -1)
    {
        std::cerr << "[F] VNORMAL NOT FOUND" << std::endl;
    }

    loc_texture = glGetAttribLocation(simConfig->program, "v_tex");
    if(loc_texture == -1)
    {
        std::cerr << "[F] VTEX NOT FOUND" << std::endl;
    }

    //renderer stuff
    loc_mvpmat = glGetUniformLocation(simConfig->program, "mvpMatrix");
    if(loc_mvpmat == -1)
    {
        std::cerr << "[F] MVPMATRIX NOT FOUND" << std::endl;
    }

    //fuck i hope this doesn't break
    for(int i=0; i<10; i++)
    {
        loc_lightPosition[i] = glGetUniformLocation(simConfig->program, const_cast<const char*>(("lights["+to_string(i)+"].position").c_str()));
        loc_lightAmbient[i] = glGetUniformLocation(simConfig->program, const_cast<const char*>(("lights["+to_string(i)+"].ambient").c_str()));
        loc_lightDiffuse[i] = glGetUniformLocation(simConfig->program, const_cast<const char*>(("lights["+to_string(i)+"].diffuse").c_str()));
        loc_lightSpecular[i] = glGetUniformLocation(simConfig->program, const_cast<const char*>(("lights["+to_string(i)+"].specular").c_str()));
        loc_lightSpotDir[i] = glGetUniformLocation(simConfig->program, const_cast<const char*>(("lights["+to_string(i)+"].spotdirection").c_str()));
        loc_lightSpotCutoff[i] = glGetUniformLocation(simConfig->program, const_cast<const char*>(("lights["+to_string(i)+"].spotcutoff").c_str()));
    }

    loc_model = glGetUniformLocation(simConfig->program, "modelMatrix");
    loc_view = glGetUniformLocation(simConfig->program, "viewMatrix");

    loc_objMatAmbient = glGetUniformLocation(simConfig->program, "lightMat.ambient");
    loc_objMatDiffuse = glGetUniformLocation(simConfig->program, "lightMat.diffuse");
    loc_objMatSpecular = glGetUniformLocation(simConfig->program, "lightMat.specular");
    loc_objShine = glGetUniformLocation(simConfig->program, "lightMat.shine");


    //unbind buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void entity::tick(double dt)
{
    int a = dt;
    a = a/1;
    //model movement stuff
    //orbitalAngle += dt * (M_PI * 2) * rotationModifier //move in a direction determined by rotationModifier, with an amount based on
     //   * (1 / ( orbitalPeriod * 24 * 60 * 60) ); //360 * dt ( seconds ) * seconds in an orbital period

    if(objPhysics.objRB!=NULL)
    {
        //poll bullet for our position now
        btTransform trans;
        objPhysics.objRB->getMotionState()->getWorldTransform(trans);

        //update position
        absolutePosition.x = trans.getOrigin().getX();
        absolutePosition.y = trans.getOrigin().getY();
        absolutePosition.z = trans.getOrigin().getZ();

        if(shape!="Plane")
	    {
            model = glm::translate( glm::mat4(1.0f), absolutePosition);
	    }
        else
            model = glm::translate( glm::mat4(1.0f), glm::vec3(0,0,0));

        //cout<<name<<endl;
        //cout<<absolutePosition.x<<"|"<<absolutePosition.y<<"|"<<absolutePosition.z<<endl;

        btScalar newZ;
        btScalar newY;
        btScalar newX;
        trans.getBasis().getEulerZYX(newZ, newY, newX, 1);

        model = glm::rotate( model, newZ , glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate( model, newY , glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate( model, newX , glm::vec3(1.0f, 0.0f, 0.0f));

        model = glm::scale( model, glm::vec3(scale, scale, scale));
    }

    /*entity* iterator = children;

    //tick each child
    while(iterator!=NULL)
    {
        iterator->tick(dt);
        iterator = iterator->next;
    }*/
}

float entity::getX()
{
    //poll bullet for our position now
    btTransform trans;
    objPhysics.objRB->getMotionState()->getWorldTransform(trans);

    //update position
    return trans.getOrigin().getX();
}

float entity::getY()
{
    //poll bullet for our position now
    btTransform trans;
    objPhysics.objRB->getMotionState()->getWorldTransform(trans);

    //update position
    return trans.getOrigin().getY();
}

float entity::getZ()
{
    //poll bullet for our position now
    btTransform trans;
    objPhysics.objRB->getMotionState()->getWorldTransform(trans);

    //update position
    return trans.getOrigin().getZ();
}

void entity::prerender()
{

}

void entity::render()
{
    //abort if we aren't visible, we shouldn't be drawing! Same goes for if we lack a rigid body!
    if(!visible || objPhysics.objRB == NULL)
        return;

    //premultiply the matrix for this example
    mvp = simConfig->projection * simConfig->view * model;

    glBindBuffer(GL_ARRAY_BUFFER, vbo_geometry);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);

    //upload the matrix to the shader
    glUniformMatrix4fv(loc_mvpmat, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniformMatrix4fv(loc_model, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(loc_view, 1, GL_FALSE, glm::value_ptr(simConfig->view));

    //set up the Vertex Buffer Object so it can be drawn
    glEnableVertexAttribArray(loc_position);
    //set pointers into the vbo for each of the attributes(position and color)
    glVertexAttribPointer( loc_position,//location of attribute
                           3,//number of elements
                           GL_FLOAT,//type
                           GL_FALSE,//normalized?
                           sizeof(Vertex),//stride
                           0);//offset

    glEnableVertexAttribArray(loc_normal);
    glVertexAttribPointer( loc_normal,//location of attribute
                           3,//number of elements
                           GL_FLOAT,//type
                           GL_FALSE,//normalized?
                           sizeof(Vertex),
                            (void*)offsetof(Vertex,normal));//offset

    glEnableVertexAttribArray(loc_texture);
    glVertexAttribPointer( loc_texture,
                           2,
                           GL_FLOAT,
                           GL_FALSE,
                           sizeof(Vertex),
                            (void*)offsetof(Vertex,uv));


    lightSource* iterator = simConfig->worldLights->head;
    for(int i=0; i<10; i++)
    {
        if(iterator!=NULL&&iterator->stateOn)
        {
            glUniform3f(loc_lightPosition[i], iterator->position[0],iterator->position[1],iterator->position[2]);
            glUniform3f(loc_lightAmbient[i], iterator->sourceAmbient[0],iterator->sourceAmbient[1],iterator->sourceAmbient[2]);
            glUniform3f(loc_lightDiffuse[i], iterator->sourceDiffuse[0],iterator->sourceDiffuse[1],iterator->sourceDiffuse[2]);
            glUniform3f(loc_lightSpecular[i], iterator->sourceSpecular[0],iterator->sourceSpecular[1],iterator->sourceSpecular[2]);
            glUniform3f(loc_lightSpotDir[i], iterator->sourceSpotlightDirection[0],iterator->sourceSpotlightDirection[1],iterator->sourceSpotlightDirection[2]);
            glUniform1f(loc_lightSpotCutoff[i], iterator->sourceSpotlightCutoff);
        }
        else
        {
            glUniform3f(loc_lightPosition[i], 0,0,0);
            glUniform3f(loc_lightAmbient[i], 0,0,0);
            glUniform3f(loc_lightDiffuse[i], 0,0,0);
            glUniform3f(loc_lightSpecular[i], 0,0,0.6);
            glUniform3f(loc_lightSpotDir[i], 0,-1,0);
            glUniform1f(loc_lightSpotCutoff[i], 90);
        }
        if(iterator!=NULL)
            iterator = iterator->next;
    }

    //enable config stuff
    //glEnableVertexAttribArray(simConfig->loc_eyeVector);
    glUniform3f(simConfig->loc_eyeVector, simConfig->currentFocalCamera->x, simConfig->currentFocalCamera->y, simConfig->currentFocalCamera->z);
    glUniform3f(loc_objMatAmbient, objLight.materialAmbient[0], objLight.materialAmbient[1], objLight.materialAmbient[2]);
//cout<<objLight.materialAmbient[0]<<objLight.materialAmbient[1]<<objLight.materialAmbient[2]<<endl;
    glUniform3f(loc_objMatDiffuse, objLight.materialDiffuse[0], objLight.materialDiffuse[1], objLight.materialDiffuse[2]);
    glUniform3f(loc_objMatSpecular, objLight.materialSpecular[0], objLight.materialSpecular[1], objLight.materialSpecular[2]);
    glUniform1f(loc_objShine, objLight.shine);

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);//mode, starting index, count

    //clean up
    glDisableVertexAttribArray(loc_position);
    glDisableVertexAttribArray(loc_normal);
    glDisableVertexAttribArray(loc_texture);
    //glDisableVertexAttribArray(simConfig->loc_eyeVector);

    //unbind buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /*entity* iterator = children;

    //draw each child
    while(iterator!=NULL)
    {
        iterator->render();
        iterator = iterator->next;
    }*/
}
