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

        bool isFont;

        //frame data
        double frameSize[2];//xy
        int frame[2];//xy, simplifies things

        std::vector< Vertex > vertices;

        GLuint vbo_sprite;// VBO handle for our geometry

        //texture data

        sprite* next;

        sprite(int nx, int ny, int nwidth, int nheight, double nscaleX = 1.0, double nscaleY = 1.0);
        ~sprite();
        void rebuild(config* simConfig);
        void setFrameData( int frameWidth, int frameHeight );
        void setFrame( int newFrameX, int newFrameY );
        void init(config* simConfig);
        void load(config* simConfig, string fpath);
        void render(  );

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
        sprite* staticSprites;
        sprite* dynamicSprites;
        sprite* font;

        spriteManager();
        ~spriteManager();

        void clearDynamic();

        void render(  );
        void init(config* simConfig);

        sprite* addSprite(config* simConfig, int nx, int ny, int nwidth, int nheight, string fpath, bool isStatic, double nscaleX = 1.0, double nscaleY = 1.0, bool useFont = false);
        void generateText(config* simConfig, string text, double scale, int startX, int startY, double gapOverride = 0);
    private:
};

#endif
