#include "config.h"
#include <iostream>
#include <fstream>
#include "entity.h"


#include "light.h"

#define pi 3.14159265

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

    worldLights = new lightArray();

    azimuthAngle = 0;
    altitudeAngle = 1;
    viewDistance = 30;
    controlRot = 0;

    lastFPS = 0;

    showDetails = false;

    //create a new camera for each of the eye positions
    Camera* newCam = new Camera();//p1
    newCam->Position(0.0,7.0,3.0);
    newCam->Rotate(90,50,14);
    newCam->controlRot = 1;
    newCam->next = presetCameras;
    presetCameras = newCam;

    newCam = new Camera();//p2
    newCam->Position(0.0,7.0,-3.0);
    newCam->Rotate(-90,50,14);
    newCam->controlRot = 2;
    newCam->next = presetCameras;
    presetCameras = newCam;

    newCam = new Camera();//center of table
    newCam->Position(0.0,6.0,0.0);
    newCam->Rotate(0,1,25);
    newCam->next = presetCameras;
    presetCameras = newCam;

    currentFocalCamera = newCam;

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
    //load in world lighting
    char dummy;
    double temp[4];
    input.clear();
    input.open("../bin/data/light_sources.dat");
    line = 0;

    lightSource* newLight = NULL;

    //give ourselves to our bff, the game data
    gameData.simConfig = this;

    input>>dummy;

    if(!input.good())
    {
        cout<<"FAILED TO LOAD LIGHTS!"<<endl;
    }

    while(input.good())
    {
        switch(line)
        {
            case 0:
                newLight = new lightSource(worldLights);

                input>>temp[0]>>dummy>>temp[1]>>dummy>>temp[2]>>dummy>>temp[3]>>dummy;

                newLight->setPosition(temp);
                line++;
                break;
            case 1:
                input>>temp[0]>>dummy>>temp[1]>>dummy>>temp[2]>>dummy>>temp[3]>>dummy;

                newLight->setAmbient(temp);
                line++;
                break;
            case 2:
                input>>temp[0]>>dummy>>temp[1]>>dummy>>temp[2]>>dummy>>temp[3]>>dummy;

                newLight->setDiffuse(temp);
                line++;
                break;
            case 3:
                input>>temp[0]>>dummy>>temp[1]>>dummy>>temp[2]>>dummy>>temp[3]>>dummy;

                newLight->setSpecular(temp);
                line++;
                break;
        }

        //call it a day with this light when we finish it
        if(line >= 4)
        {
            line = 0;
            newLight = NULL;
        }

        input>>dummy;
    }

    input.close();

    cout<<"LIGHTS LOADED!"<<endl;

    //update the camera
    recalcCamera();
}

config::~config()
{
    delete physicsEnvironment;
    delete worldLights;
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

    //set the camera-specific defaults for things
    azimuthAngle = iterator->defaultAzimuth;
    altitudeAngle = iterator->defaultAltitude;
    viewDistance = iterator->defaultDistance;
    controlRot = iterator->controlRot;

    view = glm::lookAt( glm::vec3(eyeCamera.x, eyeCamera.y, eyeCamera.z), //Eye Position
                        glm::vec3(iterator->x, iterator->y, iterator->z), //camera aim
                        glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

    currentFocalCamera = iterator;

    return iterator;
}

void config::tick(float dt)
{
    //tick the physics world
    if(physicsLimit == 0)
        physicsEnvironment->tick(dt);

    physicsLimit++;

    //tick our entities
    gameData.tick(dt);

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
