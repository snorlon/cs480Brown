#include "entityphysics.h"
#include "entity.h"

entityPhysics::entityPhysics()
{
    shape = NULL;
    simConfig = NULL;

    objMotion = NULL;
    objRB = NULL;

    mass = 1.0;//kg
}

entityPhysics::~entityPhysics()
{
    delete shape;
}

void entityPhysics::init(entity* np)
{
        parent = np;
        //create the entity's rigid body
        shape = new btBoxShape(btVector3(parent->radius, parent->radius, parent->radius));

        objMotion = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(parent->absolutePosition.x, 
            parent->absolutePosition.y, parent->absolutePosition.z)));
        btVector3 fallInertia(0, 0, 0);
        shape->calculateLocalInertia(mass, fallInertia);
        btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, objMotion, shape, fallInertia);
        objRB = new btRigidBody(fallRigidBodyCI);
        simConfig->physicsEnvironment->dynamicsWorld->addRigidBody(objRB);
}
