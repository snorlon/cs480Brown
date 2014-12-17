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

    bat = NULL;

    simConfig = NULL;

    //attempt to load in existing scores
    ifstream fin("../bin/highscores.txt");
    int i = 0;
    string input;
    int scoreIn = 0;

    gameState=1;

	objectScore.currScore = 0.0;

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
    if(objectScore.currScore>0 && gameState==2)
    {
        addScore(new score(&objectScore));
    }
    gameState=0;

    //reset the score and players
    currentGame.currScore = 120;

    time = 15;//reset our timer
}

void game::init()
{
    //load in theme
    bat = simConfig->simEntityManager->loadEntity("../bin/data/Objects/ToLoveRu/bat");

    bat->init();

    cout<<bat->absolutePosition.x<<"|"<<bat->absolutePosition.y<<"|"<<bat->absolutePosition.z<<endl;

    simConfig->presetCameras->target = bat;

    //reset the score
    resetScore();
}

void game::tick(double dt)
{
    //abort if dt is 0
    if(dt==0)
        return;

    //render the game objects if they are set
    if(bat!=NULL)
    {
        bat->tick(dt);
    }

    //do not tick if game is not running
    if(gameState==0)
    {
        time-=dt;
    }

	if(time<=0)
	{
		resetScore();
	}
}

void game::render(int renderMode)
{
    //render the game objects if they are set
    if(bat!=NULL)
    {
        bat->render(renderMode);
    }
}

void game::moveBat(double xAmount, double yAmount)
{
    //ditch controls if game not active
    if(gameState!=0)
        return;

    //restrict amount
    double threshold = 0.4;
    if(xAmount>threshold)
        xAmount = threshold;
    if(xAmount<-threshold)
        xAmount = -threshold;
    if(yAmount>threshold)
        yAmount = threshold;
    if(yAmount<-threshold)
        yAmount = -threshold;

    //move it accordingly to the amounts provided
    if(bat!=NULL)
    {
        bat->absolutePosition.x += xAmount;
        bat->absolutePosition.y = 2;
        bat->absolutePosition.z += yAmount;

        //guard the bounds for the left and right of the board
        double xLimit = 100;
        double zLimit = 100;
        if( bat->absolutePosition.x < -xLimit )
            bat->absolutePosition.x = -xLimit;
        else if( bat->absolutePosition.x > xLimit )
            bat->absolutePosition.x = xLimit;

        if( bat->absolutePosition.z < -zLimit )
            bat->absolutePosition.z = -zLimit;
        else if( bat->absolutePosition.z > zLimit )
            bat->absolutePosition.z = zLimit;
        
        btVector3 MyNewPosition( bat->absolutePosition.x, bat->absolutePosition.y, bat->absolutePosition.z );
        btVector3 vNewPos = MyNewPosition;
        btTransform btt;
        btt.setIdentity();
        bat->objPhysics.objRB->getMotionState()->getWorldTransform(btt);
        btQuaternion cOri = btt.getRotation();
        btt.setOrigin(vNewPos);
        btt.setRotation(cOri);
        bat->objPhysics.objRB->getMotionState()->setWorldTransform(btt);

        //remove the bat and readd it to the stage
        simConfig->physicsEnvironment->dynamicsWorld->removeCollisionObject( bat->objPhysics.objRB );

        //and re-add them PURIFIED
        bat->objPhysics.init( bat );
    }
}

void game::resetBall()
{
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
