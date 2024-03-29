Erin Brown's Assignment 06

========================================

Assignment explanation:

    The objective of this project is use Assimp to load models and textures for those models.

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

    Occasionally when loading a model, the path name gets jarbled and something extra gets thrown in. I think this is freeimageplus's fault
        Re-running should resolve this, no changes necessary to anything

========================================

Commands:
    All planet related keys has been disabled for this assignment

    Keyboard

        Arrow Keys - Camera movement

            Left Arrow - Move down 1 X unit
            Right Arrow - Move up 1 X unit
            Up Arrow - Move up 1 Y unit
            Down Arrow - Move down 1 Y unit

        Space - Pause/Unpause
        Esc - Exit program

    Mouse
        Right - Open menu

========================================

Extra Credit: NONE
