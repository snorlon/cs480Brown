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

    ball1 = NULL;
    ball2 = NULL;
    bat = NULL;
    table = NULL;

    simConfig = NULL;

    //attempt to load in existing scores
    ifstream fin("../bin/highscores.txt");
    int i = 0;
    string input;
    int scoreIn = 0;

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
    ball1 = simConfig->simEntityManager->loadEntity("../bin/data/Objects/Ball");
    ball2 = simConfig->simEntityManager->loadEntity("../bin/data/Objects/Ball2");
    bat = simConfig->simEntityManager->loadEntity("../bin/data/Objects/ToLoveRu/bat");
    table = simConfig->simEntityManager->loadEntity("../bin/data/Objects/Props/Labrynth");

    ball1->init();
    ball2->init();
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

    //render the game objects if they are set
    if(table!=NULL)
    {
        table->tick(dt);
    }
    if(bat!=NULL)
    {
        bat->tick(dt);
    }
    if(ball1!=NULL)
    {
        ball1->tick(dt);
        ball2->tick(dt);
        //check if the ball is out of bounds
        //if so reset it
        if(ball1->absolutePosition.y < 10)
        {
            //player lost if they fell in
            gameState=1;

            resetBall();
        }
        //check for win
        else if(ball1->absolutePosition.z <= -14.6 && gameState==0 && (ball2->absolutePosition.z <= -14.6 || mode!=1))
        {
            //player lost if they fell in
            gameState=2;
            mode = 1;//switch the mode

            resetBall();
        }

        //check if the ball is out of bounds
        //if so reset it
        else if(ball2->absolutePosition.y < 10 && mode==1)
        {
            //player lost if they fell in
            gameState=1;

            resetBall();
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
    if(ball1!=NULL)
    {
        ball1->render();
    }
    if(ball2!=NULL && mode == 1)
        ball2->render();
}

void game::moveBat(double xAmount, double yAmount)
{
    //ditch controls if game not active
    if(gameState!=0)
        return;

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
    if(ball1!=NULL)
    {
        //reset our position
        ball1->absolutePosition.x = 14.2;
        ball1->absolutePosition.y = 16;
        ball1->absolutePosition.z = -14.2;
        if(mode==1)
        {
            ball2->absolutePosition.x = 13.4;
            ball2->absolutePosition.y = 16;
            ball2->absolutePosition.z = -14.1;
        }
        else
        {
            ball2->absolutePosition.x = 0;
            ball2->absolutePosition.y = 0;
            ball2->absolutePosition.z = 0;
        }

        //remove the HEATHEN!
        simConfig->physicsEnvironment->dynamicsWorld->removeCollisionObject( ball1->objPhysics.objRB );
        simConfig->physicsEnvironment->dynamicsWorld->removeCollisionObject( ball2->objPhysics.objRB );

        //and re-add them PURIFIED
        ball1->objPhysics.init( ball1 );
        ball2->objPhysics.init( ball2 );
    }
}

void game::startGame(int nmode)
{
    mode = nmode;
    
    //to start the game we MUST
    //reset the score
    resetScore();
    resetBall();

    //reset the camera
    //simConfig->switchCamera(0);

}
