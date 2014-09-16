

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
};
