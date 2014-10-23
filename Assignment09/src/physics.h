#ifndef PHYSICSH
#define PHYSICSH

#include <bullet/LinearMath/btVector3.h>
#include <bullet/btBulletDynamicsCommon.h>

class physics
{
    public:
        btBroadphaseInterface* broadphase;
        btDefaultCollisionConfiguration* physicsConfig;
        btCollisionDispatcher* dispatcher;
        btSequentialImpulseConstraintSolver* solver;
        btDiscreteDynamicsWorld* dynamicsWorld;

        btCollisionShape* ground;

        physics();
        ~physics();
    private:
};

#endif
