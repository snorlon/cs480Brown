GROUP PROJECT's Assignment 10

Chad Brown
Alexander Falconi
Molly Headland

========================================

Assignment explanation:

    Implement a program that applies all 4 light types and allows each to be toggleable.
        Ambient
        Distant
        Point
        Spot

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
        WASD - control spotlight 1
        mouse - control spotlight 2
        arrow keys - camera

        Y - toggle ambient light
        U - toggle spotlight
        I - toggle point light
        O - toggle distant light
        P - toggle second spotlight

        Space - Pause/Unpause
        Esc - Exit program

    Mouse
        Right - Open menu
            Can pause or resume simulation or exit

========================================

Extra Credit: 
    Multiple spotlights
    Can control each spotlight

========================================

CS480 Graphics Class

Programming Assignments by GROUP
