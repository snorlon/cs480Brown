Chad Brown's Assignment 02

========================================

Assignment explanation:

    Add in keyboard events for reversing rotation direction.

    Add in right-click menu for pausing and unpausing the rotation, as well as to quit

    Add in left-click event for reversing rotation direction

========================================

Dependencies:
    freeglut
    GLM

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

    Entering custom vertex and fragment shaders is MANDATORY

    If you enter a bad filename, it will ignore it and move on.
    If the file name is good, it will say in the terminal that the corresponding shader has been loaded.

========================================

There is a small problem with hitting Escape while a menu is up that I have yet to resolve,
     it ends erroring instead of exiting cleanly due to not being able to destroy a menu that is in use.
    It works fine, however, exiting by hitting the Quit menu object.

========================================

Commands:

    Keyboard
        A - Reverse direction
        Space - Pause/Unpause
        Esc - Exit program

    Mouse
        Left - Reverse Direction
        Right - Open menu

========================================

Extra Credit: N/A

========================================

CS480 Graphics Class

Programming Assignments by Chad Brown
