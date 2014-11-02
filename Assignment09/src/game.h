#include <string>

using namespace std;

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

        int themeCount;

        game();
        void addTheme(string nname, string apath, string p1, string p2);
        void switchTheme(int themenum);
        void init();//for initing the game-specific components
    private:
};


