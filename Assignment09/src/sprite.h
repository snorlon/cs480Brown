#ifndef SPRITEH
#define SPRITEH

#include <string>

#include <GL/glew.h> // glew must be included before the main gl libs
#include <GL/glut.h> // doing otherwise causes compiler shouting

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

#include <vector>

#include <FreeImagePlus.h>

using namespace std;

class config;
class Vertex;
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

        GLint loc_texture;
        GLint loc_position;

        int texWidth;
        int texHeight;
        GLuint texID;

        std::vector< Vertex > vertices;

        GLuint vbo_sprite;// VBO handle for our geometry

        //texture data

        sprite* next;
        sprite* children;

        sprite(config* simConfig, int nx, int ny, int nwidth, int nheight, string fpath, double nscaleX = 1.0, double nscaleY = 1.0);
        ~sprite();
        void render( config* simConfig );

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

        void addSprite(config* simConfig, int nx, int ny, int nwidth, int nheight, string fpath, double nscaleX = 1.0, double nscaleY = 1.0);
    private:
};

#endif
