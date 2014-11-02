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

    For both shaders from file, enter './../bin/Project -f filename -v filename2' order does not matter
        All that matters is that file name follows the -f or -v command

    Example terminal command:
        ./../bin/Project -f ../bin/shaders/fragmentshader -v ../bin/shaders/vertexshader

    If the file name is good, it will say in the terminal that the corresponding shader has been loaded. If not, it will warn you. Shaders MUST be provided

    The terminal will tell you if your texture or model sucks
        this program only supports a single texture and a single obj and a single material file, all in the same location

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
        REWRITE

        Arrow Keys - Camera movement

            Up & Down Arrow - Rotate Along the Y plane
            Left & Right Arrow - Rotate Along the Y axis
            Pg Up & Pg Down OR
                Mousewheel up and Mousewheel down - Zooming in & out on current camera
            End - Reset zoom

        WASD - control Player 2 (if AI not enabled)

        R - reset the puck position

        + and - : controlling speed of program

        Space - Pause/Unpause
        Esc - Exit program

    Mouse
        Right - Open menu
            Can pause or resume simulation or exit
        Middle Mouse - hold to rotate camera

        Mouse - Control player 1 (if AI not enabled)

========================================

Extra Credit: 
    Detailed background
    Themes
    Lighting
    2D rendering system, including custom text rendering
    

========================================

CS480 Graphics Class

Programming Assignments by GROUP
