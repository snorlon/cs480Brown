#ifndef CONFIGH
#define CONFIGH

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

#include "shaderloader.h"
#include "entitymanager.h"

//This object will define the attributes of a vertex(position, color, etc...)
struct Vertex
{
    GLfloat position[3];
    GLfloat color[3];
};

class config
{
    public:
        //module pointers available to one and all like the filthy things they are
        shaderManager* simShaderManager;
        entityManager* simEntityManager;
        GLuint program; //program id
        float rotationModifier;

        //transform matrices
        //move later to camera object
        glm::mat4 view;//world->eye
        glm::mat4 projection;//eye->clip

        config();
        
        bool giveLinks(shaderManager* shaderMgr, entityManager* entMgr);

        //window size config data
        void setWindow( int wHeight, int wWidth);
        int getWindowHeight() { return windowHeight; }
        int getWindowWidth() { return windowWidth; }

    private:
        int windowHeight;
        int windowWidth;
};

#endif
