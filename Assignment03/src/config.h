#ifndef CONFIGH
#define CONFIGH

#include "shaderloader.h"

class config
{
    public:
        //module pointers available to one and all like the filthy things they are
        shaderManager* simShaderManager;
        GLuint program; //program id
        GLuint vbo_geometry;// VBO handle for our geometry

        config();
        
        bool giveLinks(shaderManager* shaderMgr);

        //window size config data
        void setWindow( int wHeight, int wWidth);
        int getWindowHeight() { return windowHeight; }
        int getWindowWidth() { return windowWidth; }

    private:
        int windowHeight;
        int windowWidth;
};

#endif
