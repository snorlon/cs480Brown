#ifndef ENTITYH
#define ENTITYH

#include <GL/glew.h> // glew must be included before the main gl libs
#include <GL/glut.h> // doing otherwise causes compiler shouting

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

#include "config.h"

class entity
{
    public:
        entity* next;
        glm::mat4 model;
        glm::mat4 mvp;//premultiplied modelviewprojection
        config* simConfig;
        GLuint vbo_geometry;// VBO handle for our geometry

        //planet specific stuff
        double orbitalPeriod;//how many (earth) days for a full orbit
        double orbitalAngle;//degrees into orbit
        double rotationPeriod;//how many days for a full rotation
        double rotationAngle;//degrees into its rotation
        double semimajorAxis;//diameter of orbit at most distant points
        float diameter;//Also in AU

        //uniform locations
        GLint loc_mvpmat;// Location of the modelviewprojection matrix in the shader
        //attribute locations
        GLint loc_position;
        GLint loc_color;

        entity(config* nConfig);
        ~entity();
        void init();
        void tick(float dt);
        void render();

    private:
};

#endif
