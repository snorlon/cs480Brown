#ifndef CONFIGH
#define CONFIGH

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

#include "physics.h"
#include "shaderloader.h"
#include "entitymanager.h"
#include "camera.h"
#include "game.h"


class lightArray;
//This object will define the attributes of a vertex(position, color, etc...)
struct Vertex
{
    GLfloat position[3];
    GLfloat normal[3];
    GLfloat uv[2];

    Vertex()
    {
        position[0] = 0.0;
        position[1] = 0.0;
        position[2] = 0.0;

        normal[0] = 0.0;
        normal[1] = 0.0;
        normal[2] = 0.0;

        uv[0] = 0.0;
        uv[1] = 0.0;
    }

    Vertex(double* p, double* ntex)
    {
        position[0] = p[0];
        position[1] = p[1];
        position[2] = p[2];

        normal[0] = 0.0;
        normal[1] = 0.0;
        normal[2] = 0.0;

        uv[0] = ntex[0];
        uv[1] = ntex[1];
    }

    float getX(){ return position[0]; }
    float getY(){ return position[1]; }
    float getZ(){ return position[2]; }
};

class config
{
    public:
        //module pointers available to one and all like the filthy things they are
        shaderManager* simShaderManager;
        entityManager* simEntityManager;
        GLuint program; //program id
        physics* physicsEnvironment;

        game gameData;

        lightArray* worldLights;

        Camera eyeCamera;
        Camera targetCamera;

        Camera* presetCameras;
        int currentCamera;

        int objectCount;
        int texCount;

        int azimuthAngle;
        int altitudeAngle;
        double viewDistance;

        int controlRot;

        int lastFPS;
    
        Camera* currentFocalCamera;

        bool showDetails;

        float timeRate;

        double scale;

        int physicsLimit;

        //uniform locations
        GLint loc_mvpmat;// Location of the modelviewprojection matrix in the shader
        GLint loc_eyeVector;

        //transform matrices
        //move later to camera object
        glm::mat4 view;//world->eye
        glm::mat4 projection;//eye->clip

        config();
        ~config();
        
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
