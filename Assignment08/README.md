GROUP PROJECT's Assignment 08

Erin Brown
Alexander Falconi
Molly Headland

========================================

Assignment explanation:

    Build a simple board (bottom with side walls) with 3 objects on it (a sphere, a cube, and a cylindar).
    Make the cube a static object and the sphere and cylindar dynamic objects.
    provide input so that they can move

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
        Arrow Keys - Camera movement

            Up & Down Arrow - Rotate Along the Y plane
            Left & Right Arrow - Rotate Along the Y axis
            Pg Up & Pg Down - Zooming in & out on current camera
            End - Reset zoom

        + and - : controlling speed of program        

        Space - Pause/Unpause
        Esc - Exit program

    Mouse
        Right - Open menu
            Can pause or resume simulation or exit

========================================

Extra Credit: NONE

========================================

CS480 Graphics Class

Programming Assignments by GROUP
