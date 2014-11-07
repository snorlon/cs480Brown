#ifndef ENTITYMGRH
#define ENTITYMGRH
#include <string>

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

#ifdef ASSIMP_2
#include <assimp/assimp.hpp>
#include <assimp/aiScene.h>
#include <assimp/aiPostProcess.h>
#else
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#endif

#include <FreeImagePlus.h>

class entity;
class config;

using namespace std;

class entityManager
{
    public:
        entity* head;
        int entityCount;

    
        Assimp::Importer import;

        config* simConfig;

        entityManager();
        void initialize();
        void tick(double dt);
        void loadData( string loadlistFileName );
        entity* loadEntity(string fileName);
        void prerender();//for shadows and shit
        void render();
        void cleanup();

    private:
};

#endif
