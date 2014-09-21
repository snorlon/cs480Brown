#include "entitymanager.h"
#include "entity.cpp"
#include <cstddef>
#include <iostream>

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

int entityManager::loadModels(config* simConfig, int* argc, char **argv)
{
    int loadedCount = 0;
    int attemptCount = 0;

    for(int i = 0; i < *argc; i++)
    {
        //check if we've been given a model object
        if(strcmp(argv[i], "-m")==0)
        {
            //check if this is not the last element
            if(*argc > i)
            {
                //attempt to create a new object with the file name
                entity* temp = new entity(simConfig, argv[i+1]);

                //show we tried
                attemptCount++;

                //if it fails, delete the object
                if(temp->id == -1)
                {
                    delete temp;

                    //print our FAILURE
                    std::cout<<"Failed to load model "<<attemptCount<<"!"<<std::endl;
                }

                //if it succeeds, add it to the entity manager as normal entity
                else
                {
                    temp->next = head;
                    head = temp;

                    //print our GREAT SUCCESS
                    std::cout<<"SUCCESS! Loaded model "<<attemptCount<<"!"<<std::endl;

                    //indicate success with the loaded count
                    loadedCount++;
                }
            }
        }
    }

    return loadedCount;
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
