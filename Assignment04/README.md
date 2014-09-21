Chad Brown's Assignment 04

========================================

Assignment explanation:

    Part 1: Create a model in Blender and export the OBJ file.
        Located in bin/models/board.obj
    Part 2: Model Loading: Write a model loader that can load your OBJ from Part 1 into your program.
        Three other models also available in bin/models/

========================================

Dependencies:
    freeglut
    GLM

========================================

Terminal commands in ''s

How to run:
    'Make' as normal
    enter './../bin/Project' to execute from build folder

    For loading vertex shader from file, enter './../bin/Project -v filename'

    For loading fragment shader from file, enter './../bin/Project -f filename'

    For both shaders from file, enter './../bin/Project -f filename -v filename2' order does not matter
        All that matters is that file name follows the -f or -v command

    For loading models from file, enter './../bin/Project -m filename'
        There is no limit to the number of models able to be loaded this way, but they are not moved as a result

    Example terminal command:
        ./../bin/Project -f ../bin/shaders/fragmentshader -v ../bin/shaders/vertexshader -m ../bin/models/board.obj

    WARNING: THIS MODEL LOADER REQUIRES MODELS WITH BOTH NORMALS AND UVS EXPORTED!!
        As specified by http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
        It does weird things when these are not provided!

        All models in the bin/models folder should be fully compatible with the loader, including a modified dragon.obj adjusted to fit format
            Dragon requires zooming out to see with arrow keys, others should not

    If the file name is good, it will say in the terminal that the corresponding shader has been loaded. If not, it will warn you. Shaders MUST be provided
    If it doesn't like the model given, it will not count it in the X Models Loaded printout

========================================

Bugs:

    There is a small problem with hitting Escape while a menu is up that I have yet to resolve, it ends erroring instead of exiting cleanly due to not being able to destroy a menu that is in use.
      It works fine, however, exiting by hitting the Quit menu object.

========================================

Commands:
    All planet related movement has been disabled for this assignment

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

========================================

CS480 Graphics Class

Programming Assignments by Chad Brown
