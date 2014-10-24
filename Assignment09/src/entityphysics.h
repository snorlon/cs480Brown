#ifndef ENTPHYSICSH
#define ENTPHYSICSH

#include "physics.h"
#include "config.h"

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

class entityPhysics
{
    public:
        btCollisionShape* shape;
        config* simConfig;

        btScalar mass;
        btDefaultMotionState* objMotion;
        btRigidBody* objRB;

        entityPhysics();
        ~entityPhysics();

        void init(); // use this to generate the bullet physics stuff

    private:
};

#endif
