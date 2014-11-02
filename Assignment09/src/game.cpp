#include "game.h"

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
    }
}

void game::init()
{
    addTheme("To Love Ru","ToLoveRu/", "Lala", "Momo");//our default theme

    switchTheme(1);//switch to the first theme by default
}
