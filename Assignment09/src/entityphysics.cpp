#include "entityphysics.h"
#include "entity.h"



#define BIT(x) (1<<(x))
enum collisiontypes {
    COL_NOTHING = 0, //<Collide with nothing
    COL_OBJ = BIT(0), //<Collide with ships
    COL_WALL = BIT(1), //<Collide with walls
};

int objCollidesWith = COL_WALL | COL_OBJ;
int wallCollidesWith = COL_OBJ;

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
            shape = new btBoxShape(btVector3(parent->size.x, parent->size.y, parent->size.z));
        }
        else if(parent->shape == "Cylinder")
        {
            shape = new btCylinderShape(btVector3(parent->size.x, parent->size.y, parent->size.z));
        }
        else if(parent->shape == "Plane")
        {
            //use the velocity for a normal
            shape = new btStaticPlaneShape(btVector3(parent->velocity.x,parent->velocity.y,parent->velocity.z),1);
        }
        else if(parent->shape == "Sphere")
        {
            //ez sphere creation
            shape = new btSphereShape(parent->size.x);
        }
        else if(parent->shape == "Custom")
        {
            //add all triangles to the mesh
            btTriangleMesh *triMesh = new btTriangleMesh();
            
            for(unsigned int i=0; i + 2 < parent->vertices.size(); i++)
            {
                triMesh->addTriangle(btVector3(parent->vertices[i].getX(),parent->vertices[i].getY(),parent->vertices[i].getZ()), 
                    btVector3(parent->vertices[i+1].getX(),parent->vertices[i+1].getY(),parent->vertices[i+1].getZ()),
                     btVector3(parent->vertices[i+2].getX(),parent->vertices[i+2].getY(),parent->vertices[i+2].getZ()));

                i+=2;
            }

            //generate model depending on if static or not (check mass)
            btGImpactMeshShape* bshape = new btGImpactMeshShape(triMesh);
            shape = bshape;

            shape->setLocalScaling(btVector3(parent->size.x*parent->scale, parent->size.y*parent->scale, parent->size.z*parent->scale));
            
            bshape->setMargin(0.01f);
            bshape->updateBound();
        }

        if(shape == NULL)//catch all that does things so they shouldn't have any size nearly
        {
            shape = new btBoxShape(btVector3(0.0001,0.0001,0.0001));
        }

        objMotion = new btDefaultMotionState(btTransform(btQuaternion(parent->orientation.x, parent->orientation.y, parent->orientation.z, 1), btVector3(parent->absolutePosition.x, 
            parent->absolutePosition.y, parent->absolutePosition.z)));
        btVector3 fallInertia(0, 0, 0);
        shape->calculateLocalInertia(mass, fallInertia);

        btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, objMotion, shape, fallInertia);

        //set the friction
        fallRigidBodyCI.m_friction = 0.1;

        objRB = new btRigidBody(fallRigidBodyCI);
        //simConfig->physicsEnvironment->dynamicsWorld->addRigidBody(objRB);

        //create types and add them if necessary
        if(objType == " Static")
        {
            simConfig->physicsEnvironment->dynamicsWorld->addRigidBody(objRB, COL_WALL, wallCollidesWith);
        }
        else if(objType == " Moveable")
        {
            simConfig->physicsEnvironment->dynamicsWorld->addRigidBody(objRB, COL_OBJ, objCollidesWith);
        }
	else if(objType == "Kinematic")
	{
	    objRB->setCollisionFlags(objRB->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	    objRB->setActivationState(DISABLE_DEACTIVATION);
	    simConfig->physicsEnvironment->dynamicsWorld->addRigidBody(objRB);
	}
        else
        {
            simConfig->physicsEnvironment->dynamicsWorld->addRigidBody(objRB);
        }

        //set our velocity if we have one
        objRB->setLinearVelocity(btVector3(parent->velocity.x,parent->velocity.y,parent->velocity.z));
}
