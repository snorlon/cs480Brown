#include "physics.h"


physics::physics()
{
    broadphase = new btDbvtBroadphase();
    physicsConfig = new btDefaultCollisionConfiguration();

    dispatcher = new btCollisionDispatcher(physicsConfig);

    solver = new btSequentialImpulseConstraintSolver;


    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, physicsConfig);

    //the gravity of the situation is just outrageous
    dynamicsWorld->setGravity(btVector3(0, -9.81, 0));

    //it isn't the fall that kills you, it's the collision with the ground
    ground = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
}

physics::~physics()
{
    delete broadphase;
    delete physicsConfig;
    delete dispatcher;
    delete solver;
    delete dynamicsWorld;
    delete ground;
}
