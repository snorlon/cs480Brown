#include <GL/glew.h> // glew must be included before the main gl libs
#include <GL/glut.h> // doing otherwise causes compiler shouting
#include <iostream>
#include <chrono>

#include <string.h>  //used for string comparisons for file loading
#include <iostream>
#include <fstream>

//to make compiler happy
#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

//#include <assimp/Importer.hpp>      // C++ importer interface
//#include <assimp/scene.h>           // Output data structure
//#include <assimp/postprocess.h>     // Post processing flags

#include "shaderloader.h"
#include "renderer.h"
#include "config.h"
#include "entity.h"

//--Data types

//uniform locations
GLint loc_mvpmat;// Location of the modelviewprojection matrix in the shader

//attribute locations
GLint loc_position;
GLint loc_color;

//--GLUT Callbacks
void render();
void update();
void reshape(int n_w, int n_h);
void mouse(int button, int state, int x, int y);
void keyboard(unsigned char key, int x_pos, int y_pos);
void keyboardPlus(int key, int x_pos, int y_pos);

//--Resource management
void shaderLoader(int argc, char **argv);
bool initialize();
bool postInitialize();
void cleanUp();

shaderManager simShaderManager;
renderer simRenderer;
config simConfig;
entityManager simEntities;

void menu_test(int id);

bool recentlyPaused = false;
bool keepRunning = true;

float timeRate = 86400.0f;

int menuID = -1;

//ASSIMP TESTS
// Create an instance of the Importer class
//Assimp::Importer importer;

//--Random time things
float getDT();
std::chrono::time_point<std::chrono::high_resolution_clock> t1,t2;

//--Main
int main(int argc, char **argv)
{
    //set config data
    simConfig.setWindow(480, 640);

    // Initialize glut
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize( simConfig.getWindowWidth(), simConfig.getWindowHeight());
    glutInitWindowPosition( 100, 100);
    // Name and create the Window
    glutCreateWindow("Matrix Example");

    // Now that the window is created the GL context is fully set up
    // Because of that we can now initialize GLEW to prepare work with shaders
    GLenum status = glewInit();
    if( status != GLEW_OK)
    {
        std::cerr << "[F] GLEW NOT INITIALIZED: ";
        std::cerr << glewGetErrorString(status) << std::endl;
        return -1;
    }

    // Set all of the callbacks to GLUT that we need
    //glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF); //disable key spamming for now
    glutDisplayFunc(render);// Called when its time to display
    glutReshapeFunc(reshape);// Called if the window is resized
    glutIdleFunc(update);// Called if there is nothing else to do
    glutMouseFunc(mouse);// Called if there is mouse input
    glutKeyboardFunc(keyboard);// Called if there is keyboard input
    glutSpecialFunc(keyboardPlus);// for stuff without ascii access characters like arrow keys

    //setup inputs
    menuID = glutCreateMenu(menu_test);
    glutAddMenuEntry("quit", 1);
    glutAddMenuEntry("start rotation", 2);
    glutAddMenuEntry("stop rotation", 3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);


    //spawn all entities here?
    //simEntities.createEntity(&simConfig);
    std::cout<<simEntities.loadModels(&simConfig, &argc, argv)<<" models were loaded from the command line!"<<std::endl;

    // Initialize all of our resources(config, geometry)
    bool init = initialize();

    //run our shader loader now
    init = init && simShaderManager.loadShaders(argc,argv);

    //call some other resources to initialize after the shader, separately
    init = init && postInitialize();

    if(init)
    {
        t1 = std::chrono::high_resolution_clock::now();
        glutMainLoop();
    }

    // Clean up after ourselves
    cleanUp();
    return 0;
}

void menu_test(int id)
{
    switch(id)
    {
        case 1:
            keepRunning = false;
            break;
        case 2:
            glutIdleFunc(update);
            break;
        case 3:
            glutIdleFunc(NULL);
            recentlyPaused = true;
            break;
    }
    glutPostRedisplay();
}

//--Implementations
void render()
{
    //abort execution here if we are to stop running
    if(!keepRunning)
    {
        cleanUp();
        exit(0);
    }

    //call renderer's render
    simRenderer.render();
}

void update()
{
    //time passed gets calculated
    float dt = getDT() * timeRate;

    //if we just exited a pause, just make the dt zero so no jerking when unpausing
    if(recentlyPaused)
    {
        dt = 0;
        recentlyPaused = false;
    }

    simEntities.tick(dt);

    // Update the state of the scene
    glutPostRedisplay();//call the display callback
}


void reshape(int n_w, int n_h)
{
    simConfig.setWindow( n_h, n_w );
    //Change the viewport to be correct
    glViewport( 0, 0, simConfig.getWindowWidth(), simConfig.getWindowHeight());

}

void mouse(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)//mouse left down
    {
        //flip rotation direction
        //simEntities.head->rotationModifier *= -1;
    }
}

void keyboard(unsigned char key, int x_pos, int y_pos)
{
    // Handle keyboard input
    if(key == 27)//ESC
    {
        cleanUp();
        exit(0);
    }
    else if(key == 'a')//a
    {
        //flip rotation direction
        //simEntities.head->rotationModifier *= -1;
    }
    else if(key == 61)//=
    {
        //increase time rate
        timeRate *= 2;
        if(timeRate > 10000000000)
            timeRate = 10000000000;
    }
    else if(key == 45)//-
    {
        //decrease time rate
        timeRate /= 2;
        if(timeRate < 1)
            timeRate = 1;
    }
    else if(key == ' ')//space
    {
        //toggle pause
        if(recentlyPaused)
           glutIdleFunc(update);
        else
        {
            recentlyPaused = true;
            glutIdleFunc(NULL);
        }
    }
}

void keyboardPlus(int key, int x_pos, int y_pos)
{
    // Handle keyboard input
    if(key == GLUT_KEY_LEFT)//left arrow key
    {
        simConfig.eyeCamera.DownX();
        simConfig.recalcCamera();
    }
    else if(key == GLUT_KEY_RIGHT)//left arrow key
    {
        simConfig.eyeCamera.UpX();
        simConfig.recalcCamera();
    }
    else if(key == GLUT_KEY_UP)//up arrow key
    {
        simConfig.eyeCamera.UpY();
        simConfig.recalcCamera();
    }
    else if(key == GLUT_KEY_DOWN)//down arrow key
    {
        simConfig.eyeCamera.DownY();
        simConfig.recalcCamera();
    }
}

bool initialize()
{
    bool returnVal = true;

    //initialize stuff
    //shader needs config for program stuff to setup our shaders
    returnVal = returnVal && simShaderManager.giveLinks(&simConfig);

    //config needs links to everything so it can let everyone access modules from anywhere
    returnVal = returnVal && simConfig.giveLinks(&simShaderManager, &simEntities);

    //renderer needs config for access to it and other modules
    returnVal = returnVal && simRenderer.giveLinks(&simConfig);

    return returnVal;
}

bool postInitialize()
{
    bool returnVal = true;

    //init the entity manager so it can finish what it needs to do after the shaders are in place
    simEntities.initialize();


    //separate init for old renderer stuff
    returnVal = returnVal && simRenderer.initialize();

    return returnVal;
}

void cleanUp()
{
    // Clean up, Clean up
    glutDestroyMenu( menuID );
    glDeleteProgram( simConfig.program );

    simEntities.cleanup();
}

//returns the time delta
float getDT()
{
    float ret;
    t2 = std::chrono::high_resolution_clock::now();
    ret = std::chrono::duration_cast< std::chrono::duration<float> >(t2-t1).count();
    t1 = std::chrono::high_resolution_clock::now();
    return ret;
}
