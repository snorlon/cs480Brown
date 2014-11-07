#ifndef CAMERAH
#define CAMERAH

class entity;
class Camera
{
    public:
        double x;
        double y;
        double z;

        entity* target;

        int controlRot;

        Camera();
        Camera(entity* ntarget);
        void Position(double nx, double ny, double nz);
        void MoveX(double amount);
        void MoveY(double amount);
        void MoveZ(double amount);
        void UpX(){x = x + 0.05;};
        void UpY(){y = y + 0.05;};
        void UpZ(){z = z + 0.05;};
        void DownX(){x = x - 0.05;};
        void DownY(){y = y - 0.05;};
        void DownZ(){z = z - 0.05;};
        void Rotate(int naz, int nal, int nd);

        int defaultAzimuth;
        int defaultAltitude;
        int defaultDistance;

        void tick(float dt);

        Camera* next;
};

#endif
