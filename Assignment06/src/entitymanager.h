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
        void cleanup();

    private:
};

#endif
