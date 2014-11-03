GROUP PROJECT's Assignment 00

Chad Brown
Alexander Falconi
Molly Headland

========================================

Assignment explanation:

    Air Hockey
        It does air hockey things!

    80% of graphics custom-made for this assignment, including all 3d models and most of the textures.

========================================

Dependencies:
    freeglut
    GLM
    assimp-utils
    libassimp-dev
    libassimp3
    libfreeimage3
    libfreeimage-dev
    libbullet-dev

========================================

Terminal commands in ''s

How to run:
    'Make' as normal
        For assimp2, type 'make DEFS=-DASSIMP_2'

    enter './../bin/Project' to execute from build folder

    For loading vertex shader from file, enter './../bin/Project -v filename'

    For loading fragment shader from file, enter './../bin/Project -f filename'

    For loading 2d vertex shader from file, enter './../bin/Project -2v filename'

    For loading 2d fragment shader from file, enter './../bin/Project -2f filename'

    For all shaders from file, enter './../bin/Project -f filename -v filename2 -2f filename3 -2v filename4' order does not matter
        All that matters is that file name follows the -f or -v type commands

    Example terminal command:
        ./../bin/Project -v ../bin/shaders/vertexshader -f ../bin/shaders/fragmentshader -2v ../bin/shaders/vertexshader2d -2f ../bin/shaders/fragmentshader2d

    If the file name is good, it will say in the terminal that the corresponding shader has been loaded. If not, it will warn you. Shaders MUST be provided

    The terminal will tell you if your texture or model sucks
        this program only supports a single texture and a single obj and a single material file, all in the same location

    Alternatively, run "./execute.sh" if available

========================================

Bugs:

    There is a small problem with hitting Escape while a menu is up that I have yet to resolve, it ends erroring instead of exiting cleanly due to not being able to destroy a menu that is in use.
      It works fine, however, exiting by hitting the Quit menu object.

========================================

NOTES:
    N/A

========================================

Commands:
    Keyboard
        Arrow Keys - Camera movement

            Up & Down Arrow - Rotate Along the Y plane
            Left & Right Arrow - Rotate Along the Y axis
            Pg Up & Pg Down OR
            End - Reset zoom

        WASD - control Player 2 (if AI not enabled)

        R - reset the puck position

        + and - : controlling speed of program

        1 - First person camera
        2 - Entire table camera
        3 - Second person camera

        5 - ToLoveRu theme
        6 - Pokemon theme
        7 - Mario theme
        8 - Sonic theme

        [ - toggle player 1 AI
        ] - toggle player 2 AI

        Enter - Start match

        Space - Pause/Unpause
        Esc - Exit program

    Mouse
        Right - Open menu
            Can pause or resume simulation or exit
        Middle Mouse - hold to rotate camera
            Mousewheel up and Mousewheel down - Zooming in & out on current camera

        Mouse - Control player 1 (if AI not enabled)

========================================

Extra Credit: 
    Detailed background
    Four Swappable Themes
    Lighting
    2D rendering system, including custom text rendering
    Highscores
    Two-player controls
        Rotate depending on camera
    AI for both sides
    AI/Human Toggle

========================================

CS480 Graphics Class

Programming Assignments by GROUP
