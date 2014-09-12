#include "config.h"


config::config()
{
    program = 0;
    vbo_geometry = 0;
}


bool config::giveLinks(shaderManager* shaderMgr)
{
    //abort if any provided links are bogus, we NEED them
    if(shaderMgr == NULL)
        return false;

    simShaderManager = shaderMgr;

    //assumed success accessing links
    return true;
}

void config::setWindow( int wHeight, int wWidth)
{
    windowHeight = wHeight;
    windowWidth = wWidth;
}
