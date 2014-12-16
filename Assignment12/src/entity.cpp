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
    orientation.w = 1.0;
}

entity::~entity()
{
    for(int i=0; i<shaderMax; i++)
    {
        glDeleteBuffers(1, &vbo_geometry[i]);
        glDeleteTextures(1, &texID[i]);    
    }
}

void entity::cleanup()
{


}

void entity::init()
{
    //save how much stuff is in the buffer for future rendering purposes
    vertexCount = vertices.size();
    entity* me = this;

    objPhysics.init( me );//init our bullet physics

    // Create a Vertex Buffer object to store this vertex info on the GPU


    simConfig->simShaderManager->activateShader("Depth");
    GLuint depthProgramID = simConfig->simShaderManager->activeProgram;
    depthMatrixID = glGetUniformLocation(depthProgramID, "depthMVP");
    
    //upload to every shader if possible
    for(int i=0; i<simConfig->simShaderManager->shaderCount-1; i++)
    {
        simConfig->simShaderManager->activateShader(i);
        GLint activeProgram = simConfig->simShaderManager->activeProgram;
        glGenBuffers(1, &vbo_geometry[i]);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_geometry[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

        //Now we set the locations of the attributes and uniforms
        //this allows us to access them easily while rendering
        loc_position[i] = glGetAttribLocation(activeProgram, "v_position");
        if(loc_position[i] == -1)
            std::cerr << "[F] VPOSITION NOT FOUND" << std::endl;

        loc_normal[i] = glGetAttribLocation(activeProgram, "v_normal");

        loc_texture[i] = glGetAttribLocation(activeProgram, "v_tex");

        //renderer stuff
        loc_mvpmat[i] = glGetUniformLocation(activeProgram, "mvpMatrix");

        //fuck i hope this doesn't break
        for(int j=0; j<10; j++)
        {
            loc_lightPosition[i][j] = glGetUniformLocation(activeProgram, const_cast<const char*>(("lights["+to_string(j)+"].position").c_str()));
            loc_lightAmbient[i][j] = glGetUniformLocation(activeProgram, const_cast<const char*>(("lights["+to_string(j)+"].ambient").c_str()));
            loc_lightDiffuse[i][j] = glGetUniformLocation(activeProgram, const_cast<const char*>(("lights["+to_string(j)+"].diffuse").c_str()));
            loc_lightSpecular[i][j] = glGetUniformLocation(activeProgram, const_cast<const char*>(("lights["+to_string(j)+"].specular").c_str()));
            loc_lightSpotDir[i][j] = glGetUniformLocation(activeProgram, const_cast<const char*>(("lights["+to_string(j)+"].spotdirection").c_str()));
            loc_lightSpotCutoff[i][j] = glGetUniformLocation(activeProgram, const_cast<const char*>(("lights["+to_string(j)+"].spotcutoff").c_str()));
        }

        loc_model[i] = glGetUniformLocation(activeProgram, "modelMatrix");
        loc_view[i] = glGetUniformLocation(activeProgram, "viewMatrix");

        loc_objMatAmbient[i] = glGetUniformLocation(activeProgram, "lightMat.ambient");
        loc_objMatDiffuse[i] = glGetUniformLocation(activeProgram, "lightMat.diffuse");
        loc_objMatSpecular[i] = glGetUniformLocation(activeProgram, "lightMat.specular");
        loc_objShine[i] = glGetUniformLocation(activeProgram, "lightMat.shine");


        //unbind buffer
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    }
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

void entity::render(int renderMode)
{
    //abort if we aren't visible, we shouldn't be drawing! Same goes for if we lack a rigid body!
    if(!visible || objPhysics.objRB == NULL)
        return;

    int shaderIndex = simConfig->simShaderManager->activeShader;

    //premultiply the matrix for this example
    mvp = simConfig->projection * simConfig->view * model;

    glBindBuffer(GL_ARRAY_BUFFER, vbo_geometry[shaderIndex]);


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID[shaderIndex]);

    //upload the matrix to the shader
    glUniformMatrix4fv(loc_mvpmat[shaderIndex], 1, GL_FALSE, glm::value_ptr(mvp));
    glUniformMatrix4fv(loc_model[shaderIndex], 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(loc_view[shaderIndex], 1, GL_FALSE, glm::value_ptr(simConfig->view));

    //set up the Vertex Buffer Object so it can be drawn
    glEnableVertexAttribArray(loc_position[shaderIndex]);
    //set pointers into the vbo for each of the attributes(position and color)
    glVertexAttribPointer( loc_position[shaderIndex],//location of attribute
                           3,//number of elements
                           GL_FLOAT,//type
                           GL_FALSE,//normalized?
                           sizeof(Vertex),//stride
                           0);//offset


    if(renderMode == 0)
    {
        glEnableVertexAttribArray(loc_normal[shaderIndex]);
        glVertexAttribPointer( loc_normal[shaderIndex],//location of attribute
                               3,//number of elements
                               GL_FLOAT,//type
                               GL_FALSE,//normalized?
                               sizeof(Vertex),
                                (void*)offsetof(Vertex,normal));//offset

        glEnableVertexAttribArray(loc_texture[shaderIndex]);
        glVertexAttribPointer( loc_texture[shaderIndex],
                               2,
                               GL_FLOAT,
                               GL_FALSE,
                               sizeof(Vertex),
                                (void*)offsetof(Vertex,uv));
    }


    if(renderMode == 1)
    {
		glm::vec3 lightInvDir = glm::vec3(0.5f,2,2);

		// Compute the MVP matrix from the light's point of view
		glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10,10,-10,10,-5,10);
		glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0,0,0), glm::vec3(0,1,0));
		// or, for spot light :
		//glm::vec3 lightPos(5, 20, 20);
		//glm::mat4 depthProjectionMatrix = glm::perspective<float>(45.0f, 1.0f, 2.0f, 50.0f);
		//glm::mat4 depthViewMatrix = glm::lookAt(lightPos, lightPos-lightInvDir, glm::vec3(0,1,0));

		glm::mat4 depthModelMatrix = glm::mat4(1.0);
		glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, &depthMVP[0][0]);
    }

    lightSource* iterator = simConfig->worldLights->head;
    for(int i=0; i<10; i++)
    {
        if(iterator!=NULL)
        {
            glUniform3f(loc_lightPosition[shaderIndex][i], iterator->position[0],iterator->position[1],iterator->position[2]);
            glUniform3f(loc_lightAmbient[shaderIndex][i], iterator->sourceAmbient[0],iterator->sourceAmbient[1],iterator->sourceAmbient[2]);
            glUniform3f(loc_lightDiffuse[shaderIndex][i], iterator->sourceDiffuse[0],iterator->sourceDiffuse[1],iterator->sourceDiffuse[2]);
            glUniform3f(loc_lightSpecular[shaderIndex][i], iterator->sourceSpecular[0],iterator->sourceSpecular[1],iterator->sourceSpecular[2]);
            glUniform3f(loc_lightSpotDir[shaderIndex][i], iterator->sourceSpotlightDirection[0],iterator->sourceSpotlightDirection[1],iterator->sourceSpotlightDirection[2]);
            glUniform1f(loc_lightSpotCutoff[shaderIndex][i], iterator->sourceSpotlightCutoff);
            iterator = iterator->next;
        }
        else
        {
            glUniform3f(loc_lightPosition[shaderIndex][i], 0,0,0);
            glUniform3f(loc_lightAmbient[shaderIndex][i], 0,0,0);
            glUniform3f(loc_lightDiffuse[shaderIndex][i], 0,0,0);
            glUniform3f(loc_lightSpecular[shaderIndex][i], 0,0,0.6);
            glUniform3f(loc_lightSpotDir[shaderIndex][i], 0,-1,0);
            glUniform1f(loc_lightSpotCutoff[shaderIndex][i], 90);
        }
    }

    //enable config stuff
    //glEnableVertexAttribArray(simConfig->loc_eyeVector);
    glUniform3f(simConfig->loc_eyeVector[shaderIndex], simConfig->currentFocalCamera->x, simConfig->currentFocalCamera->y, simConfig->currentFocalCamera->z);
    glUniform3f(loc_objMatAmbient[shaderIndex], objLight.materialAmbient[0], objLight.materialAmbient[1], objLight.materialAmbient[2]);
    glUniform3f(loc_objMatDiffuse[shaderIndex], objLight.materialDiffuse[0], objLight.materialDiffuse[1], objLight.materialDiffuse[2]);
    glUniform3f(loc_objMatSpecular[shaderIndex], objLight.materialSpecular[0], objLight.materialSpecular[1], objLight.materialSpecular[2]);
    glUniform1f(loc_objShine[shaderIndex], objLight.shine);

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);//mode, starting index, count

    //clean up
    glDisableVertexAttribArray(loc_position[shaderIndex]);
    glDisableVertexAttribArray(loc_normal[shaderIndex]);
    glDisableVertexAttribArray(loc_texture[shaderIndex]);
    //glDisableVertexAttribArray(simConfig->loc_eyeVector);

    //unbind buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
