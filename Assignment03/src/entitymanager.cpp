#include "entitymanager.h"
#include "entity.cpp"
#include <cstddef>

entityManager::entityManager()
{
    head = NULL;
    entityCount = 0;
}

void entityManager::initialize()
{
    entity* iterator = head;

    //init each entity
    while(iterator!=NULL)
    {
        iterator->init();
        iterator = iterator->next;
    }
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


void entityManager::render()
{
    entity* iterator = head;

    //draw each entity, let them figure out what they want done
    while(iterator!=NULL)
    {
        iterator->render();
        iterator = iterator->next;
    }
}

bool entityManager::createEntity(config* simConfig)
{
    //place holder if statement for when entity limits added
    if(true)
    {
        //push new entity to front of list
        entity* temp = new entity(simConfig);
        temp->createMoon();//give it a moon, temporary till we load it in
        //temp->satellites->createMoon();//give the moon a moon, also temporary
        temp->next = head;
        head = temp;

        entityCount++;

        return true;
    }

    //assume failure if we get here
    return false;
}

void entityManager::cleanup()
{
    //kill each entity one by one
    while(head!=NULL)
    {
        entity* temp = head;
        head = head->next;
        delete temp;
    }
}
