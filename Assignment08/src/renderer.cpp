#include "renderer.h"
#include "config.h"
#include "entity.h"

renderer::renderer()
{

}

bool renderer::giveLinks(config* configData)
{
    //abort if any provided links are bogus, we NEED them
    if(configData == NULL)
        return false;

    simConfig = configData;

    //assumed success accessing links
    return true;
}

void renderer::render()
{
    //--Render the scene

    //clear the screen
    glClearColor(0.1, 0.1, 0.15, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(simConfig->program);

    //throw in entity manager rendering
    simConfig->simEntityManager->render();

    
    //glDisable( GL_DEPTH_TEST ) ; // also disable the depth test so renders on top

    glColor3f(0.0f, 0.0f, 0.0f);
	glWindowPos2i( 20, 10 );
    char buf[300];

    //FPS
    sprintf( buf, "%s: %d", "FPS", simConfig->lastFPS ) ;
    for(unsigned int i=0;i<strlen(buf);i++)
        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, buf[i] );

    //bullet debug
    simConfig->physicsEnvironment->dynamicsWorld->debugDrawWorld();

    //Current planetary object info
    if(simConfig->currentFocalCamera != NULL && simConfig->currentFocalCamera->target != NULL && simConfig->showDetails)
    {
        entity* currentObject = simConfig->currentFocalCamera->target;

	    glWindowPos2i( 20, 425 );
        sprintf( buf, "%s", currentObject->name.c_str() ) ;
        for(unsigned int i=0;i<strlen(buf);i++)
            glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, buf[i] );

	    glWindowPos2i( 20, 400 );
        sprintf( buf, "Radius: %.1f AU", currentObject->radius ) ;
        for(unsigned int i=0;i<strlen(buf);i++)
            glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, buf[i] );

	    glWindowPos2i( 420, 350 );
        sprintf( buf, "Zoom: %.1fx", simConfig->viewDistance*100 ) ;
        for(unsigned int i=0;i<strlen(buf);i++)
            glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, buf[i] );

	    glWindowPos2i( 420, 325 );
        sprintf( buf, "Time Scale: %.1fx", simConfig->timeRate ) ;
        for(unsigned int i=0;i<strlen(buf);i++)
            glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, buf[i] );
    }


    //glEnable( GL_DEPTH_TEST ) ; // Turn depth testing back on
                           
    //swap the buffers
    glutSwapBuffers();

    //forces to operate in a finite time
    //glFlush();
}

bool renderer::initialize()
{
    //enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //and its done
    return true;
}
