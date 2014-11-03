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

#ifdef ASSIMP_2
#include <assimp/assimp.hpp>
#include <assimp/aiScene.h>
#include <assimp/aiPostProcess.h>
#else
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#endif

#include <FreeImagePlus.h>

#include "shaderloader.h"
#include "renderer.h"
#include "config.h"
#include "entity.h"

#include <bullet/LinearMath/btVector3.h>
#include <bullet/btBulletDynamicsCommon.h>

//--Data types

//uniform locations
GLint loc_mvpmat;// Location of the modelviewprojection matrix in the shader

//attribute locations
GLint loc_position;
GLint loc_color;

//mouse data
bool leftDown = false;
bool middleDown = false;

//--GLUT Callbacks
void render();
void update();
void reshape(int n_w, int n_h);
void mouse(int button, int state, int x, int y);
void mouseChange(int x, int z);
void keyboard();
void keyPressed(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
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

int menuID = -1;

int frame = 0;

bool* keyStates = new bool[256];

//--Random time things
float getDT();
std::chrono::time_point<std::chrono::high_resolution_clock> t1,t2,t3,t4;

int mbXPrior;
int mbYPrior;

double consecutivePresses[8] = {0,0,0,0,0,0,0,0};

//--Main
int main(int argc, char **argv)
{
    //set config data
    simConfig.setWindow(800, 1280);

    // Initialize glut
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize( simConfig.getWindowWidth(), simConfig.getWindowHeight());
    glutInitWindowPosition( 100, 100);
    // Name and create the Window
    glutCreateWindow("Air Hockey");
    glutFullScreen();

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
    glutMotionFunc(mouseChange);
    glutPassiveMotionFunc(mouseChange);
    glutKeyboardFunc(keyPressed);// Called if there is keyboard input
    glutKeyboardUpFunc(keyUp);// Called if there is keyboard anti-input
    glutSpecialFunc(keyboardPlus);// for stuff without ascii access characters like arrow keys

    //setup inputs
    menuID = glutCreateMenu(menu_test);
    glutAddMenuEntry("Resume", 4);
    glutAddMenuEntry("Pause", 3);
    glutAddMenuEntry("Restart", 2);
    glutAddMenuEntry("Quit", 1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    //misc setup
    simEntities.simConfig = &simConfig;

    // Initialize all of our resources(config, geometry)
    bool init = initialize();

    //load model data
    #ifdef TESTING
    simEntities.loadData("../bin/data/test_load_list.dat"); // don't want to load all those machines if I'm just testing
    #else
    simEntities.loadData("../bin/data/load_list.dat");
    #endif

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
            simConfig.gameData.startGame();
            break;
        case 4:
            //glutIdleFunc(update);
            recentlyPaused = false;
            break;
        case 3:
            //glutIdleFunc(NULL);
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
    float dt = getDT() * simConfig.timeRate;

    //rough fps handling
    frame++;
    t4 = std::chrono::high_resolution_clock::now();

    int ret = std::chrono::duration_cast< std::chrono::duration<float> >(t4-t3).count();

    //if we're paused, throw out dt
    if(recentlyPaused)
        dt = 0;

    //tick the entity manager
    simEntities.tick(dt);

    if(ret >= 1)
    {
        simConfig.lastFPS = frame;
        frame = 0;
        t3 = std::chrono::high_resolution_clock::now();

        //TICK THAT SEXY RENDERER SO IT REBUILDS THE DYNAMIC INTERFACE
        simRenderer.tick();
    }

    simConfig.tick(dt);

    //perform spam key options here
    keyboard();

    //lower the key spam counts
    double rate = 0.25;
    if(consecutivePresses[0]>0)
        consecutivePresses[0]-=rate;
    if(consecutivePresses[1]>0)
        consecutivePresses[1]-=rate;
    if(consecutivePresses[2]>0)
        consecutivePresses[2]-=rate;
    if(consecutivePresses[3]>0)
        consecutivePresses[3]-=rate;

    if(consecutivePresses[4]>0)
        consecutivePresses[4]-=rate;
    if(consecutivePresses[5]>0)
        consecutivePresses[5]-=rate;
    if(consecutivePresses[6]>0)
        consecutivePresses[6]-=rate;
    if(consecutivePresses[7]>0)
        consecutivePresses[7]-=rate;

    //cap how fast it may get
    double cap = 3.0;
    if(consecutivePresses[0]>cap)
        consecutivePresses[0]=cap;
    if(consecutivePresses[1]>cap)
        consecutivePresses[1]=cap;
    if(consecutivePresses[2]>cap)
        consecutivePresses[2]=cap;
    if(consecutivePresses[3]>cap)
        consecutivePresses[3]=cap;

    if(consecutivePresses[4]>cap)
        consecutivePresses[4]=cap;
    if(consecutivePresses[5]>cap)
        consecutivePresses[5]=cap;
    if(consecutivePresses[6]>cap)
        consecutivePresses[6]=cap;
    if(consecutivePresses[7]>cap)
        consecutivePresses[7]=cap;

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
    x=y;y=x;

    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)//mouse left down
        leftDown = true;
    else if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
        leftDown = false;
    else if(button == 3)//mousewheel up
    {
        simConfig.viewDistance -= 1.0;

        if(simConfig.viewDistance<1)
            simConfig.viewDistance = 1;
    }
    else if(button == 4)//mousewheel down
    {
        simConfig.viewDistance += 1.0;

        if(simConfig.viewDistance>95)
            simConfig.viewDistance = 95;
    }
    else if(button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
        middleDown = true;
    else if(button == GLUT_MIDDLE_BUTTON && state == GLUT_UP)
        middleDown = false;
}

void keyPressed (unsigned char key, int x, int y)
{  
    x = y;
    y = x;

    keyStates[key] = true; // Set the state of the current key to pressed  


    // Handle keyboard input
    if(key == 27)//ESC
    {
        cleanUp();
        exit(0);
    }
    else if(key == 61)//=
    {
        //increase time rate
        simConfig.timeRate *= 2;
        if(simConfig.timeRate > 65792)
            simConfig.timeRate = 65792;
    }
    else if(key == 45)//-
    {
        //decrease time rate
        simConfig.timeRate /= 2.0;
        if(simConfig.timeRate < 1)
            simConfig.timeRate = 1;
    }
    else if(key == 'r')//reset the puck position (CHEATING)
    {
        simConfig.gameData.resetPuck();
    }
    else if(key == '1')//cameras
    {
        simConfig.switchCamera(2);
    }
    else if(key == '2')//cameras
    {
        simConfig.switchCamera(0);
    }
    else if(key == '3')//cameras
    {
        simConfig.switchCamera(1);
    }
    else if(key == '5')//themes
    {
        simConfig.gameData.switchTheme(1);
    }
    else if(key == '6')//themes
    {
        simConfig.gameData.switchTheme(2);
    }
    else if(key == '7')//themes
    {
        simConfig.gameData.switchTheme(3);
    }
    else if(key == '8')//themes
    {
        simConfig.gameData.switchTheme(4);
    }
    else if(key == 'z')
    {
        simConfig.scale /= 2.0;
    }
    else if(key == 'x')
    {
        simConfig.scale = 1.0;
    }
    else if(key == 'c')
    {
        simConfig.scale *= 2.0;
    }
    else if(key == 'h')
    {
        simConfig.showDetails = !simConfig.showDetails;
    }
    //ai toggle
    else if(key == '[')
    {
        simConfig.gameData.enableAI(1);
    }
    //ai toggle
    else if(key == ']')
    {
        simConfig.gameData.enableAI(2);
    }
    else if(key == ' ')//space
    {
        //toggle pause
        if(recentlyPaused)
        {
           recentlyPaused = false;
        }
        else
        {
            recentlyPaused = true;
        }
    }
    else if(key == 13)
    {
        //start a new game
        simConfig.gameData.startGame();
    }
}  

void keyUp (unsigned char key, int x, int y)
{  
    keyStates[key] = false; // Set the state of the current key to pressed  

    x = y;
    y = x;
}  

void keyboard()
{
    float baseMovementMult = 0.1;

    //movement for bat 2(if AI not enabled)
    if(keyStates['w'])
    {
        //up the key counter to factor in holding button for speed
        consecutivePresses[0]+=1;
    }
    if(keyStates['s'])
    {
        //up the key counter to factor in holding button for speed
        consecutivePresses[1]+=1;
    }
    if(keyStates['a'])
    {
        //up the key counter to factor in holding button for speed
        consecutivePresses[2]+=1;
    }
    if(keyStates['d'])
    {
        //up the key counter to factor in holding button for speed
        consecutivePresses[3]+=1;
    }

    //update the bat positions IF time is passing
    if(!recentlyPaused)
    {
        simConfig.gameData.moveBat(1, consecutivePresses[6]*baseMovementMult - consecutivePresses[7]*baseMovementMult, consecutivePresses[4]*baseMovementMult - consecutivePresses[5]*baseMovementMult, false);
        simConfig.gameData.moveBat(2, consecutivePresses[2]*baseMovementMult - consecutivePresses[3]*baseMovementMult, consecutivePresses[0]*baseMovementMult - consecutivePresses[1]*baseMovementMult, false);
    }
}

void keyboardPlus(int key, int x_pos, int y_pos)
{
    x_pos = y_pos;
    y_pos = x_pos;
    int rate = 1;
    // Handle keyboard input
    if(key == GLUT_KEY_UP)//up arrow key
    {
        simConfig.altitudeAngle += rate;

        //artificial altitude cap, purely for airhockey, no need for more than 85
        if(simConfig.altitudeAngle>85)
            simConfig.altitudeAngle = 85;
    }
    else if(key == GLUT_KEY_DOWN)
    {
        simConfig.altitudeAngle -= rate;

        //artificial altitude cap, purely for airhockey, no need for more than 85
        if(simConfig.altitudeAngle<1)
            simConfig.altitudeAngle = 1;
    }
    else if(key == GLUT_KEY_LEFT)
    {
        simConfig.azimuthAngle -= rate;
    }
    else if(key == GLUT_KEY_RIGHT)
    {
        simConfig.azimuthAngle += rate;
    }
    else if(key == GLUT_KEY_PAGE_UP)
    {
        simConfig.viewDistance -= 1.0;

        if(simConfig.viewDistance<1)
            simConfig.viewDistance = 1;
    }
    else if(key == GLUT_KEY_PAGE_DOWN)
    {
        simConfig.viewDistance += 1.0;

        if(simConfig.viewDistance>95)
            simConfig.viewDistance = 95;
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

    //init the lights, turning them "ON"
    simConfig.worldLights->init();

    //init the game system
    simConfig.gameData.init();

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



void mouseChange(int x, int y)
{
    //detection for tracking direction with the mouse
    int xDir = 0;
    int yDir = 0;

    if(x<mbXPrior)
        xDir = -1;
    else if(x>mbXPrior)
        xDir = 1;
    if(y<mbYPrior)
        yDir = -1;
    else if(y>mbYPrior)
        yDir = 1;

    mbXPrior = x;
    mbYPrior = y;

    int rate = 1;
    if(leftDown)
    {

    }
    else if(middleDown)
    {
        //shift azimuth left
        if(xDir<0)
        {
            simConfig.azimuthAngle += rate;
        }
        else if(xDir>0) //or right
        {
            simConfig.azimuthAngle -= rate;
        }
        if(yDir<0) //shift altitude down
        {
            simConfig.altitudeAngle -= rate;

            //artificial altitude cap, purely for airhockey, no need for more than 85
            if(simConfig.altitudeAngle<1)
                simConfig.altitudeAngle = 1;
        }
        else if(yDir>0) //shift altitude up
        {
            simConfig.altitudeAngle += rate;

            //artificial altitude cap, purely for airhockey, no need for more than 85
            if(simConfig.altitudeAngle>85)
                simConfig.altitudeAngle = 85;
        }
    }
    else //passive mouse controls
    {

        //movement for bat 2(if AI not enabled)
        if(yDir==1)
        {
            //up the key counter to factor in holding button for speed
            consecutivePresses[4]+=1;
        }
        if(yDir==-1)
        {
            //up the key counter to factor in holding button for speed
            consecutivePresses[5]+=1;
        }
        if(xDir==1)
        {
            //up the key counter to factor in holding button for speed
            consecutivePresses[6]+=1;
        }
        if(xDir==-1)
        {
            //up the key counter to factor in holding button for speed
            consecutivePresses[7]+=1;
        }
    }
}
