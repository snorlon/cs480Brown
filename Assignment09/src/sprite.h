#ifndef SPRITEH
#define SPRITEH

#include <GL/glew.h> // glew must be included before the main gl libs
#include <GL/glut.h> // doing otherwise causes compiler shouting

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

class config;
class sprite
{
    public:
        //general image data
        int width;
        int height;
        int x;
        int y;
        double scaleX;
        double scaleY;

        GLuint vbo_sprite;// VBO handle for our geometry

        //texture data

        sprite* next;
        sprite* children;

        sprite();
        ~sprite();
        void render( config* simConfig );
        bool load();//not sure what will go in this yet

    private:
};

//text class derived off sprite class, ignore for now
class text: public sprite
{
    public:
        
        text();
        ~text();
    private:
        
};

class spriteManager
{
    public:
        sprite* children;

        spriteManager();
        ~spriteManager();

        void render( config* simConfig );

        bool load();//also unknown what it will require
    private:
};

#endif
