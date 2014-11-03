#include "camera.h"
#include "entity.h"

Camera::Camera()
{
    x = 0;
    y = 0;
    z = 0;

    defaultAzimuth = 0;
    defaultAltitude = 1;
    defaultDistance = 10;

    controlRot = 0;

    target = NULL;
    next = NULL;
}

Camera::Camera(entity* ntarget)
{
    x = 0;
    y = 0;
    z = 0;

    defaultAzimuth = 0;
    defaultAltitude = 1;
    defaultDistance = 10;

    controlRot = 0;

    target = ntarget;
    next = NULL;
}

void Camera::Position(double nx, double ny, double nz)
{
    x = nx;
    y = ny;
    z = nz;
}

void Camera::MoveX(double amount)
{
    x += amount;
}

void Camera::MoveY(double amount)
{
    y += amount;
}

void Camera::MoveZ(double amount)
{
    z += amount;
}

void Camera::Rotate(int naz, int nal, int nd)
{
    defaultAzimuth = naz;
    defaultAltitude = nal;
    defaultDistance = nd;
}

void Camera::tick(float dt)
{
    if(target!=NULL)
    {
        //update ourselves to where we are pointing at
        x = target->absolutePosition.x;
        y = target->absolutePosition.y;
        z = target->absolutePosition.z;
        x+= (dt*0.0);
    }
}
