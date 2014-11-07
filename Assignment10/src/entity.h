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
#include "light.h"
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
        GLuint vbo_geometry;// Vertex Buffer Object for the entire object
        GLint loc_texture;
        GLint loc_normal;
        GLint loc_position;

        GLint loc_mvpmat;

        GLint loc_model;

        GLint loc_objEmissive;
        GLint loc_objMatAmbient;
        GLint loc_objMatDiffuse;
        GLint loc_objMatSpecular;
        GLint loc_objShine;

        GLint loc_objLightsPosition;
        GLint loc_objLightsAmbient;
        GLint loc_objLightsDiffuse;
        GLint loc_objLightsSpecular;
        GLint loc_objLightsCount;

        int texWidth;
        int texHeight;
        GLuint texID;

        entityPhysics objPhysics;
        entityLight objLight;

        std::vector< Vertex > vertices;

        int vertexCount;

        string shape;

        bool visible;

        glm::mat4 mvp;//premultiplied modelviewprojection
        glm::vec3 relativePosition;//unused
        glm::vec3 absolutePosition;
        glm::vec3 velocity;
        glm::vec3 size;
        glm::vec3 orientation;

        double scale;

        //entity* children;
        entity* parent;

        //attribute locations

        entity(config* nConfig);
        ~entity();

        void cleanup();

        void init();
        void tick(double dt);

        float getX();
        float getY();
        float getZ();

        void prerender();
        void render();

    private:
};

#endif
