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
        puck->init();
    }
}

void game::tick(double dt)
{
    dt = dt;
    //render the game objects if they are set
    if(puck!=NULL)
    {
        puck->tick();
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
    if(puck!=NULL)
    {
        puck->render();
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
