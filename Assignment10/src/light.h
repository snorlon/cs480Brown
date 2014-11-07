#ifndef LIGHTH
#define LIGHTH

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier
#include <iostream>

using namespace std;

static const int SPOTLIGHT = 0;
static const int AMBIENTLIGHT = 1;
static const int POINTLIGHT = 2;
static const int DISTANTLIGHT = 3;

class entity;
class lightArray;
class lightSource
{
    public:
        entity* parent;
        lightSource* next;

        lightArray* lights;

        int id;

        int type;

        double position[4] = {0.0,0.0,0.0,0.0};
        double sourceAmbient[4] = {0.0,0.0,0.0,0.0};
        double sourceDiffuse[4] = {0.0,0.0,0.0,0.0};
        double sourceSpecular[4] = {0.0,0.0,0.0,0.0};

        bool stateOn;

        lightSource( lightArray* l, entity* p = NULL);

        void setPosition(double values[4]);
        void setAmbient(double values[4]);
        void setDiffuse(double values[4]);
        void setSpecular(double values[4]);

        void on();
        void off();

    private:
};

class lightArray
{
    public:
        lightSource* head;

        double* position;
        double* sourceAmbient;
        double* sourceDiffuse;
        double* sourceSpecular;

        int lightCount;

        lightArray();
        ~lightArray();

        void init();
        void on();
        void off();

    private:
};

//Ambient
    //output is sourceAmbient * materialAmbient
//Diffuse
    /*  direction vector = normalize(light position - surface position)
        diffuse = diffuse reflectance * diffuse of light source * max(direction * surface normal,0)
    */
//Specular
    /*  direction vector = normalize(light position - surface position)
        eye vector = normalize( eye position - surface position )
        half vector = normalize(direction vector + eye vector)
        specular = materialSpecular * sourceSpecular * (surface normal DOT half vector)^shininess
    */

class entityLightNode
{
    public:
        lightSource* node;
        entityLightNode* next;

        entityLightNode( lightSource* newL);
        ~entityLightNode();
    private:
};

class entityLight
{
    //aterial properties
    public:
//emissive, ambient, diffuse, and specular
        double emissive[4] = {0.1,0.1,0.1,0.0};
        double materialAmbient[4] = {0.1,0.1,0.1,0.0};
        double materialDiffuse[4] = {0.1,0.1,0.1,0.0};
        double materialSpecular[4] = {0.1,0.1,0.1,0.0};
        float shine; //2-128

        entityLightNode* lightNodes;

        entityLight();
        ~entityLight();

        void on();
        void off();
    private:
};

#endif
