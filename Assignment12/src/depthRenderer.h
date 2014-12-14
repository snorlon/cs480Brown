#ifndef DEPTHRENDERH
#define DEPTHRENDERH

class config;
class renderer;
class depthRenderer
{
    public:
        config* simConfig;
        renderer* renderModule;

        depthRenderer();
        ~depthRenderer();
        void render();
        bool initialize();

    private:
};


#endif
