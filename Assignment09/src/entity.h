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
#include "entityphysics.h"

using namespace std;

class entity
{
    public:
        int id;

        string name;

        entity* next;
        glm::mat4 model;
        config* simConfig;
        GLuint vbo_geometry;// VBO handle for our geometry
        //GLuint vbo_texture;// VBO handle for our texture
        GLint loc_texture;
        GLint loc_position;

        int texWidth;
        int texHeight;
        GLuint texID;

        entityPhysics objPhysics;

        std::vector< Vertex > vertices;

        int rotationModifier;//change into velocity, acceleration later?

        //planet specific stuff
        double orbitalPeriod;//how many (earth) years for a full orbit
        double orbitalAngle;//degrees into orbit
        double rotationPeriod;//how many days for a full rotation
        double rotationAngle;//degrees into its rotation
        double semimajorAxis;//diameter of orbit at most distant points
        double diameter;//Also in AU
        double tilt;//in degrees, convert to radians when rotating
        double orbitTilt;//in degrees, convert to radians
        int vertexCount;

        bool visible;

        glm::mat4 mvp;//premultiplied modelviewprojection
        glm::vec3 relativePosition;
        glm::vec3 absolutePosition;

        //entity* children;
        entity* parent;

        //attribute locations

        entity(config* nConfig);
        ~entity();

        void cleanup();

        void init();
        void tick(float dt);

        float getX();
        float getY();
        float getZ();

        void render();

    private:
};

#endif
