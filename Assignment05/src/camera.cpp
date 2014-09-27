#include "camera.h"


Camera::Camera()
{
    x = 0;
    y = 0;
    z = 0;
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
