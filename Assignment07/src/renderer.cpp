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

    
    glDisable( GL_DEPTH_TEST ) ; // also disable the depth test so renders on top

	glWindowPos2i( 20, 10 );
    char buf[300];

    //FPS
    sprintf( buf, "%s: %d", "FPS", simConfig->lastFPS ) ;
    const char * p = buf ;
    do glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, *p ); while( *(++p) ) ;

    //Current planetary object info
    if(simConfig->currentFocalCamera != NULL && simConfig->currentFocalCamera->target != NULL && simConfig->showDetails)
    {
        entity* currentObject = simConfig->currentFocalCamera->target;

	    glWindowPos2i( 20, 400 );
        sprintf( buf, "Radius: %.1f AU", currentObject->diameter / 2.0 ) ;
        const char * d = buf ;
        do glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, *d ); while( *(++d) ) ;

	    glWindowPos2i( 20, 375 );
        sprintf( buf, "Orbit Radius: %.4f AU", currentObject->semimajorAxis ) ;
        const char * o = buf ;
        do glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, *o ); while( *(++o) ) ;

	    glWindowPos2i( 20, 350 );
        sprintf( buf, "Tilt: %.4f Degrees", currentObject->tilt ) ;
        const char * t = buf ;
        do glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, *t ); while( *(++t) ) ;

	    glWindowPos2i( 20, 325 );
        sprintf( buf, "Year Duration: %.1f Days", abs(currentObject->orbitalPeriod) * 365 ) ;
        const char * r = buf ;
        do glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, *r ); while( *(++r) ) ;

	    glWindowPos2i( 20, 300 );
        sprintf( buf, "Day Duration: %.1f Hours", abs(currentObject->rotationPeriod) * 24 ) ;
        const char * a = buf ;
        do glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, *a ); while( *(++a) ) ;

	    glWindowPos2i( 420, 400 );
        sprintf( buf, "Planet Scale: %.1fx", simConfig->scale ) ;
        const char * s = buf ;
        do glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, *s ); while( *(++s) ) ;

	    glWindowPos2i( 420, 375 );
        sprintf( buf, "Zoom: %.1fx", simConfig->viewDistance*100 ) ;
        const char * v = buf ;
        do glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, *v ); while( *(++v) ) ;

	    glWindowPos2i( 420, 350 );
        sprintf( buf, "Time Scale: %.1fx", simConfig->timeRate ) ;
        const char * e = buf ;
        do glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, *e ); while( *(++e) ) ;
    }


    glEnable( GL_DEPTH_TEST ) ; // Turn depth testing back on
                           
    //swap the buffers
    glutSwapBuffers();

    glFlush();
}

bool renderer::initialize()
{
    //enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //and its done
    return true;
}
