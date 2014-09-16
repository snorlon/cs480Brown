Chad Brown's Assignment 02

========================================

Assignment explanation:

    Render two cubes onto the screen.
    One of the cubes will move around in a circle while the other cube orbits around it.
    The user, through keyboard inputs, can change the direction of rotation of the first cube, while the second continuously orbits it.

========================================

Dependencies:
    freeglut
    GLM
    (UNUSED) Assimp (not sure which in specific needed so I installed all of these).
        libassimp-dev
        assimp-utils
        libassimp-doc
        libassimp3

========================================

Terminal commands in ''s

How to run:
    'Make' as normal
    enter './../bin/Matrix' to execute from build folder

    For loading vertex shader from file, enter './../bin/Matrix -v filename'

    For loading fragment shader from file, enter './../bin/Matrix -f filename'

    For both shaders from file, enter './../bin/Matrix -f filename -v filename2' order does not matter
        All that matters is that file name follows the -f or -v command

    Example is shown in screenshot.png

    Entering custom vertex and fragment shaders is optional, and will use the pre-existing ones if they are not provided

    If you enter a bad filename, it will ignore it and move on.
    If the file name is good, it will say in the terminal that the corresponding shader has been loaded.

========================================

Bugs:

    There is a small problem with hitting Escape while a menu is up that I have yet to resolve, it ends erroring instead of exiting cleanly due to not being able to destroy a menu that is in use.
      It works fine, however, exiting by hitting the Quit menu object.

========================================

Commands:

    Keyboard
        A - Reverse planet direction
        Space - Pause/Unpause
        Esc - Exit program
        -  - Reduce time rate, divided by 2, it starts out trying to run things much faster than real time because real orbit period
        =  - Increase time rate, multiplied by 2
        Left Arrow - Rotate Planet Counter-clockwise
        Right Arrow - Rotate Planet clockwise

    Mouse
        Left - Reverse planet Direction
        Right - Open menu

========================================

Extra Credit: Present

Add text to indicate which direction the planet is rotating. This should change when the direction changes.

========================================

CS480 Graphics Class

Programming Assignments by Chad Brown
