#include "game.h"
#include "entity.h"
#include "config.h"
#include "physics.h"

score::score()
{
    player1Player = true;
    player2Player = true;

    p1Name = "Player 1";
    p2Name = "Player 2";

    score1 = 0;
    score2 = 0;
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

    simConfig = NULL;
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

    //check if our theme selection is valid
    if(index==themenum && iterator != NULL)
    {
        //if so, set it as our current theme
        currentTheme = iterator;

        //reset the score
        resetScore();
    }
}

void game::resetScore()
{
    //check if this is a high score!

    //reset the score and players
    if(currentTheme!=NULL)
    {
        currentGame.p1Name = currentTheme->p1;
        currentGame.p2Name = currentTheme->p2;
        currentGame.score1 = 0;
        currentGame.score2 = 0;
    }
}

void game::init()
{
    addTheme("To Love Ru","ToLoveRu/", "Lala", "Momo");//our default theme

    switchTheme(1);//switch to the first theme by default

    //load in our very first theme
    if(currentTheme!=NULL)
    {
        puck = simConfig->simEntityManager->loadEntity("../bin/data/Objects/"+currentTheme->assetsPath+"puck");
        bat1 = simConfig->simEntityManager->loadEntity("../bin/data/Objects/"+currentTheme->assetsPath+"bat");
        bat2 = simConfig->simEntityManager->loadEntity("../bin/data/Objects/"+currentTheme->assetsPath+"bat2");

        puck->init();
        bat1->init();
        bat2->init();
    }
}

void game::tick(double dt)
{
    dt = dt;
    //render the game objects if they are set
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
}

void game::render()
{
    //render the game objects if they are set
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
    entity* currentBat = NULL;

    //decide what bat to move based on the parameter
    if(bat == 1)
        currentBat = bat1;
    else if(bat == 2)
        currentBat = bat2;

    //move it accordingly to the amounts provided
    if(currentBat!=NULL)
    {
        currentBat->absolutePosition.x += xAmount;
        currentBat->absolutePosition.z += yAmount;

        //guard the bounds for the left and right of the board
        if( currentBat->absolutePosition.x < -4.0 )
            currentBat->absolutePosition.x = -4.0;
        else if( currentBat->absolutePosition.x > 4.0 )
            currentBat->absolutePosition.x = 4.0;

        //maintain a barrier in the center and prevent going too far behind the stage
        if(currentBat == bat1)
        {
            if( currentBat->absolutePosition.z < 1.0 )
                currentBat->absolutePosition.z = 1.0;
            else if( currentBat->absolutePosition.z > 10.0 )
                currentBat->absolutePosition.z = 10.0;
        }
        else{
            if( currentBat->absolutePosition.z > -1.0 )
                currentBat->absolutePosition.z = -1.0;
            else if( currentBat->absolutePosition.z < -10.0 )
                currentBat->absolutePosition.z = -10.0;
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
