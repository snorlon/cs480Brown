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

#include "shaderloader.h"
#include "renderer.h"


//--Data types
//This object will define the attributes of a vertex(position, color, etc...)
struct Vertex
{
    GLfloat position[3];
    GLfloat color[3];
};

//--Evil Global variables
//Just for this example!
int w = 640, h = 480;// Window size
GLuint program;// The GLSL program handle
GLuint vbo_geometry;// VBO handle for our geometry

//uniform locations
GLint loc_mvpmat;// Location of the modelviewprojection matrix in the shader

//attribute locations
GLint loc_position;
GLint loc_color;

//transform matrices
glm::mat4 model;//obj->world each object should have its own model matrix
glm::mat4 view;//world->eye
glm::mat4 projection;//eye->clip
glm::mat4 mvp;//premultiplied modelviewprojection

//--GLUT Callbacks
void render();
void update();
void reshape(int n_w, int n_h);
void mouse(int button, int state, int x, int y);
void keyboard(unsigned char key, int x_pos, int y_pos);

//--Resource management
void shaderLoader(int argc, char **argv);
bool initialize();
void cleanUp();

shaderManager simShaderManager;
Renderer simRenderer;


void menu_test(int id);

bool recentlyPaused = false;
bool keepRunning = true;
int rotationModifier = 1;

int menuID = -1;

//--Random time things
float getDT();
std::chrono::time_point<std::chrono::high_resolution_clock> t1,t2;

//--Main
int main(int argc, char **argv)
{
    simShaderManager.loadShaders(argc,argv);

    // Initialize glut
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(w, h);
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
    glutDisplayFunc(render);// Called when its time to display
    glutReshapeFunc(reshape);// Called if the window is resized
    glutIdleFunc(update);// Called if there is nothing else to do
    glutMouseFunc(mouse);// Called if there is mouse input
    glutKeyboardFunc(keyboard);// Called if there is keyboard input

    //setup inputs
    menuID = glutCreateMenu(menu_test);
    glutAddMenuEntry("quit", 1);
    glutAddMenuEntry("start rotation", 2);
    glutAddMenuEntry("stop rotation", 3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    // Initialize all of our resources(shaders, geometry)
    bool init = initialize();
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

    //enable the shader program
    glUseProgram(program);

    //call renderer's render
    simRenderer.render();
}

void update()
{
    //total time
    static float angle = 0.0;
    float dt = getDT();// if you have anything moving, use dt.

    //if we just exited a pause, just make the dt zero so no jerking when unpausing
    if(recentlyPaused)
    {
        dt = 0;
        recentlyPaused = false;
    }

    angle += (dt * M_PI/2) * rotationModifier; //move through 90 degrees a second, direction determined by rotationModifier
    model = glm::translate( glm::mat4(1.0f), glm::vec3(4.0 * sin(angle), 0.0, 4.0 * cos(angle)));

    //rotate the cube around the Y axis
    model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));

    // Update the state of the scene
    glutPostRedisplay();//call the display callback
}


void reshape(int n_w, int n_h)
{
    w = n_w;
    h = n_h;
    //Change the viewport to be correct
    glViewport( 0, 0, w, h);
    //Update the projection matrix as well
    //See the init function for an explaination
    projection = glm::perspective(45.0f, float(w)/float(h), 0.01f, 100.0f);

}

void mouse(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)//mouse left down
    {
        //flip rotation direction
        rotationModifier *= -1;
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
        rotationModifier *= -1;
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

bool initialize()
{
    bool returnVal = true;

    //initialize stuff
    returnVal = returnVal && simRenderer.giveLinks(&simShaderManager);

    returnVal = returnVal && simRenderer.initialize();

    return returnVal;
}

void cleanUp()
{
    // Clean up, Clean up
    glutDestroyMenu( menuID );
    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo_geometry);
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
