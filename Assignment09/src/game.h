#ifndef GAMEH
#define GAMEH

#include <string>

using namespace std;

class config;
class entity;
class score
{
    public:
        bool player1Player;
        string p1Name;
        bool player2Player;
        string p2Name;

        int score1;
        int score2;

        score();

    private:
};

class theme
{
    public:
        string themeName;
        string assetsPath;
        theme* next;
        string p1;
        string p2;

        theme();
    private:
};

class game
{
    public:
        score* highscores[10];
        score currentGame;

        theme* currentTheme;
        theme* themes;

        config* simConfig;

        //store game-specific materials via the game class
        entity* puck;
        entity* bat1;
        entity* bat2;

        int themeCount;

        game();
        void addTheme(string nname, string apath, string p1, string p2);
        void switchTheme(int themenum);
        void resetScore();
        void init();//for initing the game-specific components
        void tick(double dt);
        void render();
        void moveBat(int bat, double xAmount, double yAmount, bool aiControlled);

        //game-specific functions
        void resetPuck();
    private:
};

#endif
