#ifndef ENTITYH
#define ENTITYH

#include <GL/glew.h> // glew must be included before the main gl libs
#include <GL/glut.h> // doing otherwise causes compiler shouting

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

#include <vector>

#include "config.h"

using namespace std;

class entity
{
    public:
        int id;

        entity* next;
        glm::mat4 model;
        config* simConfig;
        GLuint vbo_geometry;// VBO handle for our geometry
        GLuint vbo_texture;// VBO handle for our geometry
        GLint loc_texture;
        GLint loc_position;

        int texWidth;
        int texHeight;
        GLuint texID;

        std::vector< Vertex > vertices;

        int rotationModifier;//change into velocity, acceleration later?

        //planet specific stuff
        double orbitalPeriod;//how many (earth) days for a full orbit
        double orbitalAngle;//degrees into orbit
        double rotationPeriod;//how many days for a full rotation
        double rotationAngle;//degrees into its rotation
        double semimajorAxis;//diameter of orbit at most distant points
        float diameter;//Also in AU
        int vertexCount;

        glm::vec3 relativePosition;
        glm::vec3 absolutePosition;

        //attribute locations

        entity(config* nConfig);
        ~entity();
        void init();
        void tick(float dt);

        float getX();
        float getY();

        void render();

    private:
};

#endif
