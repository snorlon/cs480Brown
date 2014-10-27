#include "config.h"
#include <iostream>
#include <fstream>
#include "entity.h"

#define pi 3.14159265

#define AU 149597871

config::config()
{
    program = 0;
    
    objectCount = 1;
    texCount = 0;

    scale = 1.0;

    physicsEnvironment = new physics();

    ifstream input;
    string data;
    input.open("../bin/data/config");
    int line = 0;

    timeRate = 1.0f;

    physicsLimit = 0;

    presetCameras = NULL;

    currentCamera = -1;

    azimuthAngle = 0;
    altitudeAngle = 35;
    viewDistance = 70;

    orbitScale = 1.0;

    lastFPS = 0;

    showDetails = false;

    currentFocalCamera = &targetCamera;
    targetCamera.Position(0.0,11.0,0.0);

    input>>data;

    while(input.good())
    {
        switch(line)
        {
            case 0:
                scale = atof(data.c_str());
                break;
        }

        input>>data;
    }

    input.close();
    

    //--Init the view (camera)
    //  if you will be having a moving camera the view matrix will need to more dynamic
    //  ...Like you should update it before you render more dynamic 
    //  for this project having them static will be fine
    eyeCamera.Position(1,1,-2);

    //update the camera
    recalcCamera();
}

config::~config()
{
    delete physicsEnvironment;
}


bool config::giveLinks(shaderManager* shaderMgr, entityManager* entMgr)
{
    //abort if any provided links are bogus, we NEED them
    if(shaderMgr == NULL)
        return false;
    if(entMgr == NULL)
        return false;

    simShaderManager = shaderMgr;
    simEntityManager = entMgr;

    std::cout<<"Config loaded!"<<std::endl;

    //set up our shader stuff
    loc_eyeVector = glGetAttribLocation(program, "v_eyeVector");

    //assumed success accessing links
    return true;
}

void config::recalcCamera()
{
    view = glm::lookAt( glm::vec3(eyeCamera.x, eyeCamera.y, eyeCamera.z), //Eye Position
                        glm::vec3(targetCamera.x, targetCamera.y, targetCamera.z), //Focus point
                        glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up
}


Camera* config::switchCamera(int camID)
{
    //abort if we don't have any preset cameras
    if(presetCameras == NULL)
        return NULL;

    Camera* iterator = presetCameras;

    while(camID>0 && iterator!=NULL)
    {
        if(iterator->next!=NULL)
        {
            iterator = iterator->next;
        }
        camID--;
    }

    view = glm::lookAt( glm::vec3(eyeCamera.x, eyeCamera.y, eyeCamera.z), //Eye Position
                        glm::vec3(iterator->x, iterator->y, iterator->z), //camera aim
                        glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

    currentFocalCamera = iterator;

    if(currentFocalCamera!=NULL && currentFocalCamera->target!=NULL)
    {
        viewDistance = 15.0;
    }

    return iterator;
}

void config::tick(float dt)
{
    //tick the physics world
    if(physicsLimit == 0)
        physicsEnvironment->tick(dt);

    physicsLimit++;
    if(physicsLimit>0)
        physicsLimit = 0;

    eyeCamera.tick(dt);
    targetCamera.tick(dt);

    Camera* iterator = presetCameras;
    while(iterator!=NULL)
    {
        iterator->tick(dt);

        iterator = iterator->next;
    }

    //prevent a negative view distance, it gets weird.
    if(viewDistance<0.0001)
        viewDistance = 0.0001;

    while(altitudeAngle<1)
        altitudeAngle+=359;
    while(altitudeAngle>=360)
        altitudeAngle-=359;
    while(azimuthAngle<1)
        azimuthAngle+=360;
    while(azimuthAngle>360)
        azimuthAngle-=360;


    int usableAltitude = altitudeAngle;
    if(altitudeAngle > 180)
        usableAltitude = -altitudeAngle;

    double eyeY = viewDistance*cos(usableAltitude * 2 * pi / 360) + currentFocalCamera->y;

    double eyeX = viewDistance*sin(usableAltitude * 2 * pi / 360)*cos(azimuthAngle * 2 * pi / 360) + currentFocalCamera->x;

    double eyeZ = viewDistance*sin(usableAltitude * 2 * pi / 360)*sin(azimuthAngle * 2 * pi / 360) + currentFocalCamera->z;

//cout<<altitudeAngle<<"|"<<eyeX-currentFocalCamera->x<<"|"<<eyeY-currentFocalCamera->y<<"|"<<eyeZ-currentFocalCamera->z<<endl;

    view = glm::lookAt( glm::vec3(eyeX, eyeY, eyeZ), //Eye Position
                        glm::vec3(currentFocalCamera->x, currentFocalCamera->y, currentFocalCamera->z), //camera aim
                        glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up
/*
        cout<<endl;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
            cout<< view[i][j]<<"|";
        cout<<endl;
    }
        cout<<endl;*/
}

void config::setWindow( int wHeight, int wWidth)
{
    windowHeight = wHeight;
    windowWidth = wWidth;

    //Update the projection matrix when we set the window size
    //FOV, ratio, min distance, max distance
    projection = glm::perspective(45.0f, float(windowWidth)/float(windowHeight), 0.001f, 10000.0f);
}
