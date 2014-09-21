#ifndef ENTITYMGRH
#define ENTITYMGRH

class entity;
class config;

class entityManager
{
    public:
        entity* head;
        int entityCount;

        entityManager();
        void initialize();
        void tick(float dt);
        void render();
        int loadModels(config* simConfig, int* argc, char **argv); //returns number of models loaded this way
        void cleanup();

    private:
};

#endif
