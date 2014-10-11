#ifndef CONFIGH
#define CONFIGH

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

#include "shaderloader.h"
#include "entitymanager.h"
#include "camera.h"

//This object will define the attributes of a vertex(position, color, etc...)
struct Vertex
{
    GLfloat position[3];
    GLfloat uv[2];
};

class config
{
    public:
        //module pointers available to one and all like the filthy things they are
        shaderManager* simShaderManager;
        entityManager* simEntityManager;
        GLuint program; //program id

        Camera eyeCamera;
        Camera targetCamera;

        Camera* presetCameras;
        int currentCamera;

        int objectCount;
        int texCount;

        Camera* currentFocalCamera;

        double scale;

        //uniform locations
        glm::mat4 mvp;//premultiplied modelviewprojection
        GLint loc_mvpmat;// Location of the modelviewprojection matrix in the shader
        //transform matrices
        //move later to camera object
        glm::mat4 view;//world->eye
        glm::mat4 projection;//eye->clip

        config();
        
        bool giveLinks(shaderManager* shaderMgr, entityManager* entMgr);
        void recalcCamera();

        Camera* switchCamera(int camID);

        void tick(float dt);

        //window size config data
        void setWindow( int wHeight, int wWidth);
        int getWindowHeight() { return windowHeight; }
        int getWindowWidth() { return windowWidth; }

    private:
        int windowHeight;
        int windowWidth;
};

#endif
