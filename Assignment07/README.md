GROUP PROJECT's Assignment 07

Erin Brown
Alexander Falconi
Molly Headland

========================================

Assignment explanation:

    The objective of this project is to build an interactive solar system.

    Use Assimp to load models and textures for the planets/moons

    Use a data file for info on each object (size, orbit radius, rotation,...)

========================================

Dependencies:
    freeglut
    GLM
    assimp-utils
    libassimp-dev
    libassimp3
    libfreeimage3
    libfreeimage-dev

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

    For loading models from file, enter './../bin/Project -m filename'
        I do not recommend adding more than one model in total this way, it gets weird and I need to fix this later

    Example terminal command:
        ./../bin/Project -f ../bin/shaders/fragmentshader -v ../bin/shaders/vertexshader -m ../bin/models/board.obj

    If the file name is good, it will say in the terminal that the corresponding shader has been loaded. If not, it will warn you. Shaders MUST be provided
    If it doesn't like the model given, it will not count it in the X Models Loaded printout

    The terminal will tell you if your texture or model sucks
        this program only supports a single texture and a single obj and a single material file, all in the same location

========================================

Bugs:

    There is a small problem with hitting Escape while a menu is up that I have yet to resolve, it ends erroring instead of exiting cleanly due to not being able to destroy a menu that is in use.
      It works fine, however, exiting by hitting the Quit menu object.

========================================

NOTES:
    Sun is significantly downscaled to hit a balance between planet size and distance between planets. Despite this, it is still not too small to be seen as anything but the sun.

    Some orbits have been rescaled, such as Earth's moon, so that they are still visible and not overlapping with the larger model scaling.

========================================

Commands:
    Keyboard

        Arrow Keys - Camera movement

            Up & Down Arrow - Rotate Along the Y plane
            Left & Right Arrow - Rotate Along the Y axis
            Pg Up & Pg Down - Zooming in & out on current camera
            End - Reset zoom

        Number keys 1-9
            Switch between each of the 9 planets as camera focus
        Number ky 0
            Re-center back on the sun

        + and - : controlling speed of program

        H - hide/unhide additional information about current planet

        Z - Divide planet scale by 10
        X - Reset planet scale to 1
        C - Multiply planet scale by 10

        A - Add distance scale by 2
        S - Reset distance scale to 1
        D - Subtract distance scale by 2
        

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
