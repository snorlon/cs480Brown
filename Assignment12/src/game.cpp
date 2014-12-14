#include "game.h"
#include "entity.h"
#include "config.h"
#include "physics.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>

using namespace std;

score::score()
{
    currScore = 0;
}

score::score(score* s)
{
    currScore = s->currScore;
}

theme::theme()
{
    themeName = "None";
    assetsPath = "ERROR";
    next = NULL;
    p1 = "Player 1";
    p2 = "player 2";
}

game::game()
{
    //we should start with 0 high scores
    for(int i=0; i<10; i++)
        highscores[i] = NULL;

    ball = NULL;
    bat = NULL;
    table = NULL;

    simConfig = NULL;

    //attempt to load in existing scores
    ifstream fin("../bin/highscores.txt");
    int i = 0;
    string input;
    int scoreIn = 0;

    ai1Enabled = false;
    ai2Enabled = false;

    gameState=1;

    srand (0);

    fin>>scoreIn;
    while(i<10 && fin.good())
    {
        highscores[i] = new score();
        highscores[i]->currScore = scoreIn;

        fin>>scoreIn;
        i++;
    }
    fin.close();
}

game::~game()
{
    //save out our high scores on exit
    ofstream fout("../bin/highscores.txt");
    //output all of the highscores
    for(int i=0; i<10; i++)
    {
        //check if the high score exists
        if(highscores[i]!=NULL)
        {
            fout<<highscores[i]->currScore<<endl;
        }
    }
    
    fout.close();
}

void game::addScore(score* s)
{
    //check if this is a high score!
    score* current = s;

    //check if we beat someone else
    for(int i=0; i<10&&highscores[i]!=NULL; i++)
    {
        //if so
        if(highscores[i]->currScore < current->currScore)
        {
            //ursurp their puny score
            score* temp = highscores[i];
            highscores[i] = current;
            current = temp;

            //now add on this old score
            addScore(current);
            return;
        }
    }
    //if that failed, then we check if there's an empty space for us
    for(int i=0; i<10;i++)
    {
        if(highscores[i] == NULL)
        {
            highscores[i] = current;
            return;//abort here
        }
    }

    //otherwise, purge the scum
    delete current;
}

void game::resetScore()
{
    //only add successful scores
    if(currentGame.currScore>0 && gameState==2)
    {
        addScore(new score(&currentGame));
    }
    gameState=0;

    //reset the score and players
    currentGame.currScore = 120;

    time = 0;//reset our timer
}

void game::init()
{
    //load in theme
    ball = simConfig->simEntityManager->loadEntity("../bin/data/Objects/Ball");
    bat = simConfig->simEntityManager->loadEntity("../bin/data/Objects/ToLoveRu/bat");
    table = simConfig->simEntityManager->loadEntity("../bin/data/Objects/Props/Labrynth");

    ball->init();
    bat->init();
    table->init();

    //reset the score
    resetScore();

    //TEMPORARY
    //toggle ai on
    //enableAI(1);
    //enableAI(2);
}

void game::tick(double dt)
{
    //abort if dt is 0
    if(dt==0)
        return;
    //do our rolls up here so that they go even if they aren't used
    double roll1 = ((rand() % 21)-10)/10;
    double roll2 = ((rand() % 21)-10)/10;

    //render the game objects if they are set
    if(table!=NULL)
    {
        table->tick(dt);
    }
    if(bat!=NULL)
    {
        bat->tick(dt);
    }
    if(ball!=NULL)
    {
        ball->tick(dt);
        //check if the puck is out of bounds
        //if so reset it
        if(ball->absolutePosition.y < 10)
        {
            //player lost if they fell in
            gameState=1;

            resetBall();
        }
        //check for win
        else if(ball->absolutePosition.z <= -14.6 && gameState==0)
        {
            //player lost if they fell in
            gameState=2;

            resetBall();
        }
    }
    //do ai things
    if(ai2Enabled||ai1Enabled)
    {
        double timeStep = 0.5;
        double moveCap = 0.5;


        //predict where puck should be
        btVector3  forceAddedVel = ball->objPhysics.objRB->getLinearVelocity();

         //the predicted position is the balls position plus these two terms
        btTransform pucktrans;
        ball->objPhysics.objRB->getMotionState()->getWorldTransform(pucktrans);

        btTransform bat1trans;

        if(ai1Enabled)
        {
            bat->objPhysics.objRB->getMotionState()->getWorldTransform(bat1trans);
        }

        //update position
        btVector3 FuturePosition=pucktrans.getOrigin() + (forceAddedVel) * timeStep;

        //add a small factor to try to get behind
        //also add a small amount of error
        if(ai1Enabled)
        {
            double xDifference = FuturePosition.getX()-bat1trans.getOrigin().getX()-0.5+(roll1/100);
            double zDifference = FuturePosition.getZ()-bat1trans.getOrigin().getZ()+0.5+(roll2/10);

            if(xDifference > moveCap)
                xDifference = moveCap;
            if(xDifference < -moveCap)
                xDifference = -moveCap;
            if(zDifference > moveCap)
                zDifference = moveCap;
            if(zDifference < -moveCap)
                zDifference = -moveCap;

            //attempt to move towards it
            moveBat(1, -zDifference, xDifference, true);
        }
    }

    //do not tick if game is not running
    if(gameState==0)
    {
        time+=dt;
        //also change our score
        if(currentGame.currScore>0)
        {
            currentGame.currScore-=dt;
        }
        if(currentGame.currScore<0)
            currentGame.currScore=0;
    }
}

void game::render()
{
    //render the game objects if they are set
    if(table!=NULL)
    {
        table->render();
    }
    if(bat!=NULL)
    {
        bat->render();
    }
    if(ball!=NULL)
    {
        ball->render();
    }
}

void game::moveBat(int cbat, double xAmount, double yAmount, bool aiControlled)
{
    //ditch controls if game not active
    if(gameState!=0)
        return;

    //check if we aren't supposed to be able to control this bat
    if(!aiControlled && cbat==1 && ai1Enabled)
        return;//abort, we can't touch this

    entity* currentBat = NULL;

    //decide what bat to move based on the parameter
    currentBat = bat;

    //move it accordingly to the amounts provided
    if(currentBat!=NULL)
    {
        currentBat->absolutePosition.x += xAmount;
        currentBat->absolutePosition.z += yAmount;

        //guard the bounds for the left and right of the board
        double xLimit = 3.2;
        double yLimit = 9.5;
        if( currentBat->absolutePosition.x < -xLimit )
            currentBat->absolutePosition.x = -xLimit;
        else if( currentBat->absolutePosition.x > xLimit )
            currentBat->absolutePosition.x = xLimit;

        //maintain a barrier in the center and prevent going too far behind the stage
        if( currentBat->absolutePosition.z < 1.0 )
            currentBat->absolutePosition.z = 1.0;
        else if( currentBat->absolutePosition.z > yLimit )
            currentBat->absolutePosition.z = yLimit;

        
        btVector3 MyNewPosition( currentBat->absolutePosition.x, currentBat->absolutePosition.y, currentBat->absolutePosition.z );
        btVector3 vNewPos = MyNewPosition;
        btTransform btt;
        btt.setIdentity();
        currentBat->objPhysics.objRB->getMotionState()->getWorldTransform(btt);
        btQuaternion cOri = btt.getRotation();
        btt.setOrigin(vNewPos);
        btt.setRotation(cOri);
        currentBat->objPhysics.objRB->getMotionState()->setWorldTransform(btt);

        //remove the bat and readd it to the stage
        //simConfig->physicsEnvironment->dynamicsWorld->removeCollisionObject( currentBat->objPhysics.objRB );

        //and re-add them PURIFIED
        //currentBat->objPhysics.init( currentBat );
    }
}

void game::resetBall()
{
    if(ball!=NULL)
    {
        //reset our position
        ball->absolutePosition.x = 14.2;
        ball->absolutePosition.y = 16;
        ball->absolutePosition.z = -14.2;

        //remove the HEATHEN!
        simConfig->physicsEnvironment->dynamicsWorld->removeCollisionObject( ball->objPhysics.objRB );

        //and re-add them PURIFIED
        ball->objPhysics.init( ball );
    }
}

void game::startGame()
{
    //to start the game we MUST
    //reset the score
    resetScore();
    resetBall();

    //reset the camera
    //simConfig->switchCamera(0);

}
