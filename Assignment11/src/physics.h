#include <bullet/LinearMath/btVector3.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>

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

        btDefaultMotionState* groundMotionState;
        btRigidBody* groundRigidBody;


        physics();
        ~physics();

        void tick(double dt);
    private:
};

#endif
