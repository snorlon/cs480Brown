#include "physics.h"


physics::physics()
{
    physicsWorld = new btDbvtBroadphase();
}
