GROUP PROJECT's Assignment 12

Chad Brown
Alexander Falconi
Molly Headland

========================================

Assignment explanation:

    Final project.
    You are a present bumping into buildings to move them to score points.
    Includes buggy shadow mapping.

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

        WASD - control present

        R - reset the balls position

        + and - : controlling speed of program

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
    N/A

========================================

CS480 Graphics Class

Programming Assignments by GROUP
