#ifndef GAMEH
#define GAMEH

#include <string>

using namespace std;

class config;
class entity;
class score
{
    public:
        double currScore;

        score();
        score(score* s);

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

        theme currentTheme;

        config* simConfig;

        int mode;

        //store game-specific materials via the game class
        entity* ball1;
        entity* ball2;
        entity* bat;
        entity* table;

        double time;

        int gameState;

        game();
        ~game();
        void addScore(score* s);
        void resetScore();
        void init();//for initing the game-specific components
        void tick(double dt);
        void render();
        void moveBat(double xAmount, double yAmount);
        void startGame(int nmode = 0);

        //game-specific functions
        void resetBall();
    private:
};

#endif
