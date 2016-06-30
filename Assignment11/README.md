GROUP PROJECT's Assignment 11

Erin Brown
Alexander Falconi
Molly Headland

========================================

Assignment explanation:

    The purpose of this assignment is to produce a 3D simulation of the Labyrinth game that is as realistic to the actual game as possible, yet still being a playable and an enjoyable experience for the end user.

    -Labyrinth

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

    No command-line input, just "./../bin/Project" after making. All needed files are loaded in on startup.

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

        WASD - control balls

        R - reset the balls position

        + and - : controlling speed of program

        1 - Game camera
        2 - Table camera

        5 - Start 1-ball game
        6 - Start 2-ball game

        Enter - Start match with prior ball count

        Space - Pause/Unpause
        Esc - Exit program

    Mouse
        Right - Open menu
            Can pause or resume simulation or exit
        Middle Mouse - hold to rotate camera
            Mousewheel up and Mousewheel down - Zooming in & out on current camera

========================================

Extra Credit: 
    Multiple Balls
    High Scores

========================================

CS480 Graphics Class

Programming Assignments by GROUP
