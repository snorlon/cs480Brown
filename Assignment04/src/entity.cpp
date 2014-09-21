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
    orbitalPeriod = 365.256363004 / 100; //default to earths rate for now
    rotationPeriod = 0.99726968; //default to earths rate for now
    semimajorAxis = 2.00000261 * 6; //AU, earths for now
    diameter = 25000* 12742 / AU;



    //temp_vertices is for storing vertices as we load them in from their definitions file
    std::vector< glm::vec3 > temp_vertices;

    //this is to store the vertices when they are defining faces
    std::vector< Vertex > vertices;

    //this is the end Vertex storage that goes to the buffer
    Vertex* verticesFinal;

    //open the file
    FILE * file = fopen(filename, "r");
    if( file == NULL ){
        cout<<"BAD FILE NAME FOR MODEL PROVIDED!"<<endl;
        id = -1; //set id so we know the model was a big failure
        return;
    }

    while( 1 )
    {
        char lineHead[256];
        //read the first word of the line, should be a v, vt, vn, f, etc
        int fileReadCode = fscanf(file, "%s", lineHead);
        if (fileReadCode == EOF)
        {
            //abort when we finish going through the file peacefully
            break;
        }
     
        //Check if this line is a vertex
        if ( strcmp( lineHead, "v" ) == 0 )
        {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            temp_vertices.push_back(vertex);
        }
        //Check if this line is a vertex texture
        else if ( strcmp( lineHead, "vt" ) == 0 )
        {
            //we don't want it so just pull it and skip over it
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y );
        }
        //Check if this line is a vertex normal
        else if ( strcmp( lineHead, "vn" ) == 0 )
        {
            //we don't want it so just pull it and skip over it
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
        }
        //Otherwise, check if it's a face
        else if ( strcmp( lineHead, "f" ) == 0 )
        {
            //we're only going to use the vertexIndex, but we're pulling uvIndex and normalIndex out as well while at it
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

            //grab data from it in the format v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
            //if we don't like the format, abort and skip to next line
            if (matches != 9)
            {
                //set id so we know this model got fucked up
                id = -1;
                continue;
            }

            //push in each of the three vertices involved in the face onto a vertices stack to be copied later
            for(int i=0;i<3; i++)
            {
                Vertex v;
                v.position[0] = temp_vertices[ vertexIndex[i]-1 ].x;
                v.position[1] = temp_vertices[ vertexIndex[i]-1 ].y;
                v.position[2] = temp_vertices[ vertexIndex[i]-1 ].z;
                v.color[0] = (rand() % 101)/100.0f;
                v.color[1] = (rand() % 101)/100.0f;
                v.color[2] = (rand() % 101)/100.0f;
                vertices.push_back(v);
            }
        }
    }

    //create a regular Vertex array to store all of the vertices and faces in before handing it to the buffer for permanent storage
    verticesFinal = new Vertex[vertices.size()];

    for( unsigned int i=0; i<vertices.size(); i++ )
    {
        verticesFinal[i].position[0] = vertices[i].position[0];
        verticesFinal[i].position[1] = vertices[i].position[1];
        verticesFinal[i].position[2] = vertices[i].position[2];
        verticesFinal[i].color[0] = vertices[i].color[0];
        verticesFinal[i].color[1] = vertices[i].color[1];
        verticesFinal[i].color[2] = vertices[i].color[2];
    }

    //save how much stuff is in the buffer for future rendering purposes
    vertexCount = vertices.size();
    
    // Create a Vertex Buffer object to store this vertex info on the GPU
    glGenBuffers(1, &vbo_geometry);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_geometry);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesFinal) * vertices.size() * 3, verticesFinal, GL_STATIC_DRAW);
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
    //DO NOTHING IN TICK, WE ARE NOT A PLANET DO NOT BE REDICULOUS
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
