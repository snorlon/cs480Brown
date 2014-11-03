#include "game.h"
#include "entity.h"
#include "config.h"
#include "physics.h"
#include <iostream>
#include <fstream>

using namespace std;

score::score()
{
    player1Player = true;
    player2Player = true;

    p1Name = "Player 1";
    p2Name = "Player 2";

    score1 = 0;
    score2 = 0;
}

score::score(score* s)
{
    player1Player = s->player1Player;
    player2Player = s->player2Player;

    p1Name = s->p1Name;
    p2Name = s->p2Name;

    score1 = s->score1;
    score2 = s->score2;
}

string score::bestName()
{
    //output the better of the two players
    string p = p1Name;

    if(score2 > score1)
    {
        p = p2Name;
    }

    return p;
}

int score::bestScore()
{
    //output the better of the two players
    int s = score1;

    if(score2 > score1)
    {
        s = score2;
    }

    return s;
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

    currentTheme = NULL;
    themes = NULL;
    themeCount = 0;

    puck = NULL;
    bat1 = NULL;
    bat2 = NULL;
    table = NULL;

    simConfig = NULL;

    //attempt to load in existing scores
    ifstream fin("../bin/highscores.txt");
    int i = 0;
    string input;
    int scoreIn = 0;

    getline(fin, input, '|');//get the name
    while(i<10 && fin.good())
    {
        highscores[i] = new score();
        //set it to player 1 by default
        highscores[i]->p1Name = input;
        fin>>scoreIn;
        highscores[i]->score1 = scoreIn;

        getline(fin, input);//get the end of the line
        getline(fin, input, '|');//get the name
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
            fout<<highscores[i]->bestName()<<"|"<<highscores[i]->bestScore()<<endl;
        }
    }
    
    fout.close();
}

void game::addTheme(string nname, string apath, string p1, string p2)
{
    theme* newtheme = new theme();
    newtheme->themeName = nname;
    newtheme->assetsPath = apath;
    newtheme->p1 = p1;
    newtheme->p2 = p2;

    if(themes == NULL)
    {
        themes = newtheme;
    }
    else
    {
        theme* iterator = themes;
        while(iterator->next!=NULL)
            iterator = iterator->next;

        iterator->next = newtheme;
    }
    themeCount++;
}

void game::switchTheme(int themenum)
{
    if(themenum>themeCount)
        themenum = themeCount;

    int index = 1;
    theme* iterator = themes;
    while(index<themenum && iterator!=NULL)
    {
        iterator = iterator->next;
        index++;
    }

    gameActive = false;

    //check if our theme selection is valid
    if(index==themenum && iterator != NULL)
    {
        //if so, set it as our current theme
        currentTheme = iterator;

        //load in theme
        puck = simConfig->simEntityManager->loadEntity("../bin/data/Objects/"+currentTheme->assetsPath+"puck");
        bat1 = simConfig->simEntityManager->loadEntity("../bin/data/Objects/"+currentTheme->assetsPath+"bat");
        bat2 = simConfig->simEntityManager->loadEntity("../bin/data/Objects/"+currentTheme->assetsPath+"bat2");
        table = simConfig->simEntityManager->loadEntity("../bin/data/Objects/"+currentTheme->assetsPath+"table");

        puck->init();
        bat1->init();
        bat2->init();
        table->init();

        //reset the score
        resetScore();
    }
}

void game::addScore(score* s)
{
    //check if this is a high score!
    score* current = s;

    //check if we beat someone else
    for(int i=0; i<10&&highscores[i]!=NULL; i++)
    {
        //if so
        if(highscores[i]->bestScore() < current->bestScore())
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
    if(currentGame.bestScore()>0)
        addScore(new score(&currentGame));

    //reset the score and players
    if(currentTheme!=NULL)
    {
        currentGame.p1Name = currentTheme->p1;
        currentGame.p2Name = currentTheme->p2;
        currentGame.score1 = 0;
        currentGame.score2 = 0;
    }

    time = 60;//reset our timer
}

void game::init()
{
    addTheme("To Love Ru","ToLoveRu/", "Lala", "Momo");//our default theme

    switchTheme(1);//switch to the first theme by default
}

void game::tick(double dt)
{
    dt = dt;
    //render the game objects if they are set
    if(table!=NULL)
    {
        table->tick(dt);
    }
    if(bat1!=NULL)
    {
        bat1->tick(dt);
    }
    if(bat2!=NULL)
    {
        bat2->tick(dt);
    }
    if(puck!=NULL)
    {
        puck->tick(dt);
        //check if the puck is out of bounds
        //if so reset it
        if(puck->absolutePosition.y < 1)
        {
            //make sure to update the scoreboard!
            //base score on z position
            if(puck->absolutePosition.z > 5.0)
                currentGame.score2++;
            if(puck->absolutePosition.z < 5.0)
                currentGame.score1++;

            resetPuck();
        }
    }

    //do not tick if game is not running
    if(gameActive)
        time-=dt;

    //check if timer is zero
    //end the game if so
    if(time<=0)
        gameActive = false;
}

void game::render()
{
    //render the game objects if they are set
    if(table!=NULL)
    {
        table->render();
    }
    if(bat1!=NULL)
    {
        bat1->render();
    }
    if(bat2!=NULL)
    {
        bat2->render();
    }
    if(puck!=NULL)
    {
        puck->render();
    }
}

void game::moveBat(int bat, double xAmount, double yAmount, bool aiControlled)
{
    //ditch controls if game not active
    if(!gameActive)
        return;

    entity* currentBat = NULL;

    //decide what bat to move based on the parameter
    if(bat == 1)
        currentBat = bat1;
    else if(bat == 2)
        currentBat = bat2;

    //tweak amounts based on bat, AI, and angle
    switch(simConfig->controlRot)
    {
        case 0:
            //bat modifications
            if(bat == 1)
            {
                double temp = xAmount;
                xAmount = yAmount;
                yAmount = -temp;
            }
            else
            {
                double temp = xAmount;
                xAmount = -yAmount;
                yAmount = temp;
            }
            break;
        case 1:
            //bat modifications
            if(bat == 2)
            {
                xAmount = -xAmount;
                yAmount = -yAmount;
            }
            break;
        case 2:
            //bat modifications
            if(bat == 1)
            {
                xAmount = -xAmount;
                yAmount = -yAmount;
            }
            break;
    }

    //move it accordingly to the amounts provided
    if(currentBat!=NULL)
    {
        currentBat->absolutePosition.x += xAmount;
        currentBat->absolutePosition.z += yAmount;

        //guard the bounds for the left and right of the board
        double xLimit = 3.0;
        double yLimit = 9.0;
        if( currentBat->absolutePosition.x < -xLimit )
            currentBat->absolutePosition.x = -xLimit;
        else if( currentBat->absolutePosition.x > xLimit )
            currentBat->absolutePosition.x = xLimit;

        //maintain a barrier in the center and prevent going too far behind the stage
        if(currentBat == bat1)
        {
            if( currentBat->absolutePosition.z < 1.0 )
                currentBat->absolutePosition.z = 1.0;
            else if( currentBat->absolutePosition.z > yLimit )
                currentBat->absolutePosition.z = yLimit;
        }
        else{
            if( currentBat->absolutePosition.z > -1.0 )
                currentBat->absolutePosition.z = -1.0;
            else if( currentBat->absolutePosition.z < -yLimit )
                currentBat->absolutePosition.z = -yLimit;
        }

        
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

void game::resetPuck()
{
    if(puck!=NULL)
    {
        //reset our position
        puck->absolutePosition.x = 0;
        puck->absolutePosition.y = 10;
        puck->absolutePosition.z = 0;

        //remove the HEATHEN!
        simConfig->physicsEnvironment->dynamicsWorld->removeCollisionObject( puck->objPhysics.objRB );

        //and re-add them PURIFIED
        puck->objPhysics.init( puck );
    }
}

void game::startGame()
{
    //to start the game we MUST
    //set the flag
    gameActive = true;

    //reset the score
    resetScore();

    //reset the camera
    simConfig->switchCamera(0);

}
