#include "physics.h"


physics::physics()
{
    broadphase = new btDbvtBroadphase();
    physicsConfig = new btDefaultCollisionConfiguration();

    dispatcher = new btCollisionDispatcher(physicsConfig);

    solver = new btSequentialImpulseConstraintSolver;


    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, physicsConfig);

    //the gravity of the situation is just outrageous
    dynamicsWorld->setGravity(btVector3(0, -100.81, 0));

    //it isn't the fall that kills you, it's the collision with the ground
    /*ground = new btStaticPlaneShape(btVector3(0, 1, 0), 1);

    //add our ground to bullet
    //give it a position
    groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1.0, 0)));
    //create rigid body construction information
    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, ground, btVector3(0, 0, 0));
    //create the actual rigid body
    groundRigidBody = new btRigidBody(groundRigidBodyCI);
    //and shove that rigid body up into bullet for use
    dynamicsWorld->addRigidBody(groundRigidBody);*/

    btCollisionDispatcher * disp = static_cast<btCollisionDispatcher *>(dynamicsWorld->getDispatcher());
    btGImpactCollisionAlgorithm::registerAlgorithm(disp);
}

physics::~physics()
{
    //delete broadphase; //DO NOT DELETE, COMPUTER HATES IT.
    delete physicsConfig;
    delete dispatcher;
    delete solver;
    //delete dynamicsWorld; //THIS TOO. BAD. DON't DO IT. SERIOUSLY. PLS.
    delete ground;

    delete groundMotionState;
    delete groundRigidBody;
}

void physics::tick(double dt)
{
    //update the bullet physics
    if(dt>0.0)
        dynamicsWorld->stepSimulation(dt, 10);
}
