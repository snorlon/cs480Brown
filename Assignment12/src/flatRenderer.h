#ifndef FLATRENDH
#define FLATRENDH
#include "sprite.h"

class config;
class renderer;
class flatRenderer
{
    public:
        config* simConfig;
        renderer* renderModule;

        GLuint texID;
        GLuint fbo_drawn;
        GLuint renderedTexture;

        sprite* flatRenderSprite;


        spriteManager spriteModule;//holds all 2d image data

        flatRenderer();
        ~flatRenderer();

        bool initialize();
        void render();
    private:
};

#endif
