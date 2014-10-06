

class Camera
{
    public:
        double x;
        double y;
        double z;

        Camera();
        void Position(double nx, double ny, double nz);
        void MoveX(double amount);
        void MoveY(double amount);
        void MoveZ(double amount);
        void UpX(){x = x + 1;};
        void UpY(){y = y + 1;};
        void UpZ(){z = z + 1;};
        void DownX(){x = x - 1;};
        void DownY(){y = y - 1;};
        void DownZ(){z = z - 1;};
};
