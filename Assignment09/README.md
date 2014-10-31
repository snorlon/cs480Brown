GROUP PROJECT's Assignment 08

Chad Brown
Alexander Falconi
Molly Headland

========================================

Assignment explanation:

	   DO THIS LATER

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

	The puck doesn't fit through the slot in the table.
	Sometimes the puck glitches through the table.

========================================

Controls:
	Keyboard
		Arrow Keys - Camera movement

		      Up & Down Arrow - Rotate Along the Y plane
		      Left & Right Arrow - Rotate Along the Y axis
            	      Pg Up & Pg Down - Zooming in & out on current camera
            	      End - Reset zoom
       Mouse
		Drag mouse button to control bat.<br>

========================================

Other things:

      If you want to test without waiting for the arcade machine to load in:
>$make DEFS=-DTESTING

*This also changes the zoom in/out from page up/down to F1/F2 because I don't have page up/down on my laptop, sorry*
