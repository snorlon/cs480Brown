#include <bullet/LinearMath/btVector3.h>
#include <bullet/btBulletDynamicsCommon.h>

#ifndef PHYSICSH
#define PHYSICSH

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
