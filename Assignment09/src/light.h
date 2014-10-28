#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

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

        int type;

        double position[4];
        double sourceAmbient[4];
        double sourceDiffuse[4];
        double sourceSpecular[4];

        bool stateOn;

        lightSource();
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
        lightSource* next;

    private:
};

class entityLight
{
    public:
//emissive, ambient, diffuse, and specular
        double emissive[4];
        double materialAmbient[4];
        double materialDiffuse[4];
        double materialSpecular[4];
        float shine; //2-128

        entityLight();

        void on();
        void off();
    private:
};
