#include "entitymanager.h"
#include "entity.cpp"
#include <cstddef>

entityManager::entityManager()
{
    head = NULL;
}

void entityManager::tick(float dt)
{
    entity* iterator = head;

    //tick each entity, let them figure out what they want done
    while(iterator!=NULL)
    {
        iterator->tick(dt);
        iterator = iterator->next;
    }
}
