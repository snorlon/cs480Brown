#ifndef ENTITYMGRH
#define ENTITYMGRH

class entity;
class config;

class entityManager
{
    public:
        int entityCount;

        entityManager();
        void initialize();
        void tick(float dt);
        void render();
        bool createEntity(config* simConfig);
        void cleanup();

    private:
        entity* head;
};

#endif
