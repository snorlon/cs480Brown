#include "entity.h"

class entityManager
{
    public:
        entityManager();
        void tick(float dt);

    private:
        entity* head;
};
