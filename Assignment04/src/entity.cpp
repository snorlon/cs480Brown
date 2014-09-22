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

entity::entity(config* nConfig, char* filename) //load a model from a file
{
    srand (0);//set random for now to preset
    next = NULL;
    simConfig = nConfig;

    id = simConfig->objectCount;
    simConfig->objectCount++;

    orbitalAngle = 0.0;
    rotationAngle = 0.0;

    rotationModifier = 1.0f;

    //1 AU = 149,597,871 KM
    orbitalPeriod = 1; //default to earths rate for now
    rotationPeriod = 5.0; //default to earths rate for now
    semimajorAxis = 0.0; //AU, earths for now
    diameter = 12000* 12742 / AU;



    //temp_vertices is for storing vertices as we load them in from their definitions file
    std::vector< glm::vec3 > temp_vertices;

    //this is to store the vertices when they are defining faces
    std::vector< Vertex > vertices;

    //open the file
    ifstream input;
    input.clear();
    input.open(filename);

    if( !input.good()){
        cout<<"BAD FILE NAME FOR MODEL PROVIDED!"<<endl;
        id = -1; //set id so we know the model was a big failure
        return;
    }

    char* lineHead = new char[256];
    input >> std::ws;
    input.getline(lineHead,256,' ');

    while( input.good() )
    {     
        //Check if this line is a vertex
        if ( strcmp( lineHead, "v" ) == 0 )
        {
            glm::vec3 vertex;
            //fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            input>>vertex.x;
            input>>vertex.y;
            input>>vertex.z;
            temp_vertices.push_back(vertex);
        }
        //Otherwise, check if it's a face
        else if ( strcmp( lineHead, "f" ) == 0 )
        {
            //we're only going to use the vertexIndex, but we're pulling uvIndex and normalIndex out as well while at it
            unsigned int vertexIndex[3];

            input>>vertexIndex[0];
            input.getline(lineHead,256,' ');

            input>>vertexIndex[1];
            input.getline(lineHead,256,' ');

            input>>vertexIndex[2];
            input.getline(lineHead,256,'\n');

            //push in each of the three vertices involved in the face onto a vertices stack to be copied later
            for(int i=0;i<3; i++)
            {
                Vertex v;
                v.position[0] = temp_vertices[ vertexIndex[i]-1 ].x;
                v.position[1] = temp_vertices[ vertexIndex[i]-1 ].y;
                v.position[2] = temp_vertices[ vertexIndex[i]-1 ].z;
                float color = (rand() % 101)/100.0f;
                v.color[0] = 0.0f;
                v.color[1] = 0.0f;
                v.color[2] = color;
                vertices.push_back(v);
            }
        }
        else
        {
            input.getline(lineHead,256);//finish off the line
        }
        input >> std::ws;
        input.getline(lineHead,256,' ');
    }

    //save how much stuff is in the buffer for future rendering purposes
    vertexCount = vertices.size();
    
    // Create a Vertex Buffer object to store this vertex info on the GPU
    glGenBuffers(1, &vbo_geometry);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_geometry);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
}

entity::~entity()
{
    glDeleteBuffers(1, &vbo_geometry);
}

void entity::init()
{
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
