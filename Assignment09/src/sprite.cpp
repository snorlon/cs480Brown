#include "sprite.h"
#include "config.h"


//start sprite data members
sprite::sprite()
{
    next = NULL;
    children = NULL;
    
    width = 10;
    height = 10;
    x = 0;
    y = 0;
    scaleX = 1.0;
    scaleY = 1.0;

    glGenBuffers(1, &vbo_sprite);
}

sprite::~sprite()
{
    while(children!=NULL)
    {
        sprite* temp = children;
        children = children->next;
        delete temp;
    }
}

void sprite::render( config* simConfig )
{
    glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
      glColor3f(1.0f, 0.0f, 0.0f); // Red
      glVertex2f(10.8f, 0.1f);     // Define vertices in counter-clockwise (CCW) order
      glVertex2f(0.2f, 0.1f);     //  so that the normal (front-face) is facing you
      glVertex2f(0.2f, 10.7f);
      glVertex2f(10.8f, 10.7f);
    glEnd();
}


//start sprite manager data members
spriteManager::spriteManager()
{
    children = NULL;
}

spriteManager::~spriteManager()
{
    while(children!=NULL)
    {
        sprite* temp = children;
        children = children->next;
        delete temp;
    }
}

void spriteManager::render( config* simConfig )
{
    const int XSize = simConfig->getWindowWidth();
    const int YSize = simConfig->getWindowHeight();
 
    //exit 3d rendering temporarily

    glMatrixMode (GL_PROJECTION);

    glOrtho (0, XSize, YSize, 0, 0, 1);

    glDisable(GL_DEPTH_TEST);

    glMatrixMode (GL_MODELVIEW);

    // Displacement trick for exact pixelization

    glTranslatef(0.375, 0.375, 0);

    sprite* iterator = children;
    while(iterator!=NULL)
    {
        iterator->render(simConfig);
        iterator = iterator->next;
    }

    //enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

bool spriteManager::load()
{
    children = new sprite();

    return true;
}
