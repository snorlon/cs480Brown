#ifndef ENTPHYSICSH
#define ENTPHYSICSH

#include "physics.h"
#include "config.h"

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

class entity;
class entityPhysics
{
    public:
        btCollisionShape* shape;
        config* simConfig;
        entity* parent;
        string objType;

        btScalar mass;
        btDefaultMotionState* objMotion;
        btRigidBody* objRB;

        entityPhysics();
        ~entityPhysics();

        void init(entity* np); // use this to generate the bullet physics stuff

    private:
};

#endif
