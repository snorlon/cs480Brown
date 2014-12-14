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

const int shaderMax = 500;

class entity
{
    public:
        int id;

        string name;

        entity* next;
        glm::mat4 model;
        config* simConfig;
        GLuint vbo_fragment;// Vertex Buffer Object for the entire object in fragment shader
        GLuint vbo_depth;// Vertex Buffer Object for the entire object in fragment shader
        GLint loc_texture[shaderMax];
        GLint loc_normal[shaderMax];
        GLint loc_position[shaderMax];

        GLint loc_biasmvp[shaderMax];
        GLint loc_mvpmat[shaderMax];

        GLint loc_model[shaderMax];
        GLint loc_view[shaderMax];

        GLint loc_objMatAmbient[shaderMax];
        GLint loc_objMatDiffuse[shaderMax];
        GLint loc_objMatSpecular[shaderMax];
        GLint loc_objShine[shaderMax];

        int texWidth;
        int texHeight;
        GLuint texID[shaderMax];

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
        glm::vec4 orientation;

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
        void uploadVertices(int index, bool enable=true);
        void drawVertices(int index);
        void depthRender();

    private:
};

#endif
