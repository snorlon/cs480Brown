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

    id = simConfig->objectCount;
    simConfig->objectCount++;

    orbitalAngle = (110*id/360)*(M_PI * 2);
    rotationAngle = (140*id/360)*(M_PI * 2);

    rotationModifier = 1.0f;

    //1 AU = 149,597,871 KM
    orbitalPeriod = 1; //default to earths rate for now
    rotationPeriod = 5.0; //default to earths rate for now
    semimajorAxis = 0.0; //AU, earths for now
    diameter = 12000* 12742 / AU;
}

entity::~entity()
{
    glDeleteBuffers(1, &vbo_geometry);
}

void entity::init()
{
    //save how much stuff is in the buffer for future rendering purposes
    vertexCount = vertices.size();
    
    // Create a Vertex Buffer object to store this vertex info on the GPU
    glGenBuffers(1, &vbo_geometry);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_geometry);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    //Now we set the locations of the attributes and uniforms
    //this allows us to access them easily while rendering
    simConfig->loc_position = glGetAttribLocation(simConfig->program,
                    const_cast<const char*>("v_position"));
    if(simConfig->loc_position == -1)
    {
        std::cerr << "[F] VPOSITION NOT FOUND" << std::endl;
    }

    simConfig->loc_color = glGetAttribLocation(simConfig->program,
                    const_cast<const char*>("v_color"));
    if(simConfig->loc_color == -1)
    {
        std::cerr << "[F] VCOLOR NOT FOUND" << std::endl;
    }

    //renderer stuff
    simConfig->loc_mvpmat = glGetUniformLocation(simConfig->program,
                    const_cast<const char*>("mvpMatrix"));
    if(simConfig->loc_mvpmat == -1)
    {
        std::cerr << "[F] MVPMATRIX NOT FOUND" << std::endl;
    }

    std::cout << "Object initialized." << std::endl;
}

void entity::tick(float dt)
{
    //model movement stuff
    orbitalAngle += dt * (M_PI * 2) * rotationModifier //move in a direction determined by rotationModifier, with an amount based on
        * (1 / ( orbitalPeriod * 24 * 60 * 60) ); //360 * dt ( seconds ) * seconds in an orbital period


    //update relative and absolute position
    relativePosition.x = semimajorAxis * sin(orbitalAngle);
    relativePosition.y = 0.0f;
    relativePosition.z = semimajorAxis * cos(orbitalAngle);
    absolutePosition = relativePosition;


    //math specific to orbits, translate based on absolutePosition
    model = glm::translate( glm::mat4(1.0f), absolutePosition);

    //rotate the cube around the Y axis
    rotationAngle += dt * (M_PI * 2) * rotationModifier //move in a direction determined by rotationModifier, with an amount based on
        * (1 / ( rotationPeriod * 24 * 60 * 60) ); //360 * dt ( seconds ) * seconds in an orbital period
    model = glm::rotate(model,(float) rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));

    //apply the scale
    model = glm::scale( model, glm::vec3(diameter));
}

float entity::getX()
{
    float returnValue = semimajorAxis * sin(orbitalAngle) / diameter;

    return returnValue;
}

float entity::getY()
{
    float returnValue = semimajorAxis * cos(orbitalAngle) / diameter;

    return returnValue;
}

void entity::render()
{

    //premultiply the matrix for this example
    simConfig->mvp = simConfig->projection * simConfig->view * model;



    //upload the matrix to the shader
    glUniformMatrix4fv(simConfig->loc_mvpmat, 1, GL_FALSE, glm::value_ptr(simConfig->mvp));

    //set up the Vertex Buffer Object so it can be drawn
    glEnableVertexAttribArray(simConfig->loc_position);
    glEnableVertexAttribArray(simConfig->loc_color);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_geometry);
    //set pointers into the vbo for each of the attributes(position and color)
    glVertexAttribPointer( simConfig->loc_position,//location of attribute
                           3,//number of elements
                           GL_FLOAT,//type
                           GL_FALSE,//normalized?
                           sizeof(Vertex),//stride
                           0);//offset

    glVertexAttribPointer( simConfig->loc_color,
                           3,
                           GL_FLOAT,
                           GL_FALSE,
                           0,
                           (void*)offsetof(Vertex,color));

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);//mode, starting index, count

    //clean up
    glDisableVertexAttribArray(simConfig->loc_position);
    glDisableVertexAttribArray(simConfig->loc_color);
}
