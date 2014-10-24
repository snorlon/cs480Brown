#include "entityphysics.h"

entityPhysics::entityPhysics()
{
    shape = new btBoxShape(btVector3(0.25, 0.25, 0.25));
    simConfig = NULL;

    objMotion = NULL;
    objRB = NULL;

    mass = 1.0;//kg
}

entityPhysics::~entityPhysics()
{
    delete shape;
}

void entityPhysics::init()
{
        //create the entity's rigid body
        objMotion = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));
        btVector3 fallInertia(0, 0, 0);
        shape->calculateLocalInertia(mass, fallInertia);
        btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, objMotion, shape, fallInertia);
        objRB = new btRigidBody(fallRigidBodyCI);
        simConfig->physicsEnvironment->dynamicsWorld->addRigidBody(objRB);
}
