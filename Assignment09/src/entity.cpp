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
    simConfig->simShaderManager->activate3DShaders();
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

    loc_model = glGetUniformLocation(simConfig->program, "modelMatrix");

    loc_objEmissive = glGetUniformLocation(simConfig->program, "v_obj_emissive");
    loc_objMatAmbient = glGetUniformLocation(simConfig->program, "v_obj_ambient");
    loc_objMatDiffuse = glGetUniformLocation(simConfig->program, "v_obj_diffuse");
    loc_objMatSpecular = glGetUniformLocation(simConfig->program, "v_obj_specular");
    loc_objShine = glGetUniformLocation(simConfig->program, "v_obj_shine");

    loc_objLightsPosition = glGetUniformLocation(simConfig->program, "v_lights_positions");
    loc_objLightsAmbient = glGetUniformLocation(simConfig->program, "v_lights_ambient");
    loc_objLightsDiffuse = glGetUniformLocation(simConfig->program, "v_lights_diffuse");
    loc_objLightsSpecular = glGetUniformLocation(simConfig->program, "v_lights_specular");
    loc_objLightsCount = glGetUniformLocation(simConfig->program, "v_lights_count");


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

    //upload the lights
    glEnableVertexAttribArray(loc_objLightsPosition);
    glVertexAttribPointer( loc_objLightsPosition,//location of attribute
                           simConfig->worldLights->lightCount*4,//number of elements
                           GL_FLOAT,//type
                           GL_FALSE,//normalized?
                           0,//stride
                           0);//offset
    glEnableVertexAttribArray(loc_objLightsAmbient);
    glVertexAttribPointer( loc_objLightsAmbient,//location of attribute
                           simConfig->worldLights->lightCount*4,//number of elements
                           GL_FLOAT,//type
                           GL_FALSE,//normalized?
                           sizeof( simConfig->worldLights->sourceAmbient),//stride
                           0);//offset
    glEnableVertexAttribArray(loc_objLightsDiffuse);
    glVertexAttribPointer( loc_objLightsDiffuse,//location of attribute
                           simConfig->worldLights->lightCount*4,//number of elements
                           GL_FLOAT,//type
                           GL_FALSE,//normalized?
                           sizeof( simConfig->worldLights->sourceDiffuse),//stride
                           0);//offset
    glEnableVertexAttribArray(loc_objLightsSpecular);
    glVertexAttribPointer( loc_objLightsSpecular,//location of attribute
                           simConfig->worldLights->lightCount*4,//number of elements
                           GL_FLOAT,//type
                           GL_FALSE,//normalized?
                           sizeof( simConfig->worldLights->sourceSpecular),//stride
                           0);//offset

    glUniform1f(loc_objLightsCount, simConfig->worldLights->lightCount);

    /*loc_objLightsPosition = glGetUniformLocation(simConfig->program, "v_lights_positions");
    loc_objLightsAmbient = glGetUniformLocation(simConfig->program, "v_lights_ambient");
    loc_objLightsDiffuse = glGetUniformLocation(simConfig->program, "v_lights_diffuse");
    loc_objLightsSpecular = glGetUniformLocation(simConfig->program, "v_lights_specular");
    loc_objLightsSpecular = glGetUniformLocation(simConfig->program, "v_lights_count");*/

    //enable config stuff
    //glEnableVertexAttribArray(simConfig->loc_eyeVector);
    glUniform4f(simConfig->loc_eyeVector, simConfig->currentFocalCamera->x, simConfig->currentFocalCamera->y, simConfig->currentFocalCamera->z, 1);
    glUniform4f(loc_objMatAmbient, objLight.materialAmbient[0], objLight.materialAmbient[1], objLight.materialAmbient[2], objLight.materialAmbient[3]);
//cout<<objLight.materialAmbient[0]<<objLight.materialAmbient[1]<<objLight.materialAmbient[2]<<endl;
    glUniform4f(loc_objMatDiffuse, objLight.materialDiffuse[0], objLight.materialDiffuse[1], objLight.materialDiffuse[2], objLight.materialDiffuse[3]);
    glUniform4f(loc_objMatSpecular, objLight.materialSpecular[0], objLight.materialSpecular[1], objLight.materialSpecular[2], objLight.materialSpecular[3]);
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
