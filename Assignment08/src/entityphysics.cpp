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
        //mix things up based on how they're shaped
        if(parent->shape == "Box")
        {
            shape = new btBoxShape(btVector3(parent->radius, parent->radius, parent->radius));
        }
        else if(parent->shape == "Cylinder")
        {
            shape = new btCylinderShape(btVector3(parent->radius, parent->radius, parent->radius));
        }
        else if(parent->shape == "Plane")
        {
            //use the velocity for a normal
            shape = new btStaticPlaneShape(btVector3(parent->velocity.x,parent->velocity.y,parent->velocity.z),1);
        }
        else if(parent->shape == "Sphere")
        {
            //ez sphere creation
            shape = new btSphereShape(parent->radius*2);
        }
        else//catch all that does things so they shouldn't have any size nearly
        {
            shape = new btBoxShape(btVector3(0.0001,0.0001,0.0001));
        }

        objMotion = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(parent->absolutePosition.x, 
            parent->absolutePosition.y, parent->absolutePosition.z)));
        btVector3 fallInertia(0, 0, 0);
        shape->calculateLocalInertia(mass, fallInertia);

        btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, objMotion, shape, fallInertia);
        objRB = new btRigidBody(fallRigidBodyCI);
        simConfig->physicsEnvironment->dynamicsWorld->addRigidBody(objRB);

        //set our velocity if we have one
        objRB->setLinearVelocity(btVector3(parent->velocity.x,parent->velocity.y,parent->velocity.z));
}
