#include "renderer.h"
#include "config.h"
#include "entity.h"

renderer::renderer()
{
}

bool renderer::giveLinks(config* configData)
{
    //abort if any provided links are bogus, we NEED them
    if(configData == NULL)
        return false;

    simConfig = configData;

    //assumed success accessing links
    return true;
}

void renderer::render()
{
    //--Render the scene
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_main);
    glViewport(0,0,simConfig->getWindowWidth(),simConfig->getWindowHeight());

    //activate 3D shader
    if(simConfig->lightPerVertex)
        simConfig->simShaderManager->activateShader("PerVertexLighting");
    else
        simConfig->simShaderManager->activateShader("PerFragmentLighting");

    //clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //perform pre-rendering
    simConfig->simEntityManager->prerender();

    //throw in entity manager rendering
    simConfig->simEntityManager->render();

    //render game objects
    simConfig->gameData.render();


    //Will the real frame buffer please stand up
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    //glClearColor(0.5, 0.5, 0.9, 1.0);
    glViewport(0,0,simConfig->getWindowWidth(),simConfig->getWindowHeight());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //activate 2D shader
    simConfig->simShaderManager->activateShader("2D");


    //render the 2d images
    sprites.render(  );

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, renderedTexture);
		// Set our "renderedTexture" sampler to user Texture Unit 0
		glUniform1i(texID, 0);

glBindBuffer(GL_ARRAY_BUFFER, outputSprite->vbo_sprite);

    //set up the Vertex Buffer Object so it can be drawn
    glEnableVertexAttribArray(outputSprite->loc_position);
    //set pointers into the vbo for each of the attributes(position and color)
    glVertexAttribPointer( outputSprite->loc_position,//location of attribute
                           3,//number of elements
                           GL_FLOAT,//type
                           GL_FALSE,//normalized?
                           sizeof(Vertex),//stride
                           0);//offset

    glEnableVertexAttribArray(outputSprite->loc_texture);
    glVertexAttribPointer( outputSprite->loc_texture,
                           2,
                           GL_FLOAT,
                           GL_FALSE,
                           sizeof(Vertex),
                            (void*)offsetof(Vertex,uv));

    glDrawArrays(GL_QUADS, 0, 4);//mode, starting index, count

    //clean up
    glDisableVertexAttribArray(outputSprite->loc_position);
    glDisableVertexAttribArray(outputSprite->loc_texture);

    //unbind buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
                           
    //swap the buffers
    glutSwapBuffers();

    //forces to operate in a finite time
    glFlush();
}

void renderer::tick()
{
    sprites.clearDynamic();

    //BRING IN THE DYNAMICS
    //create our text stuffs
    //sprites.addSprite(simConfig, 300, 200, 85, 232, "testtrainer.png", false);
    sprites.generateText(simConfig, "High Scores", 0.4, 45, 215);
    sprites.generateText(simConfig, to_string(int(ceil(simConfig->gameData.time))), 0.5, 612, 730);
    sprites.generateText(simConfig, "FPS "+to_string(simConfig->lastFPS), 0.4, 25, 740);
    sprites.generateText(simConfig, "Version 1.0.0", 0.4, 985, 740);

    //scoreboard stuff
    sprites.generateText(simConfig, ""+to_string(int(floor(simConfig->gameData.currentGame.currScore))), 0.5, 425, 740);

    //ai flags
    if(simConfig->gameData.ai1Enabled)
        sprites.generateText(simConfig, "AI ENABLED", 0.3, 370, 700,-2);
    if(simConfig->gameData.ai2Enabled)
        sprites.generateText(simConfig, "AI ENABLED", 0.3, 740, 700,-2);

    //render GAMEOVER if game is not active
    if(simConfig->gameData.gameState==2)
        sprites.generateText(simConfig, "YOU WIN!", 1.0, 410, 440, 30);
    else if(simConfig->gameData.gameState==1)
        sprites.generateText(simConfig, "YOU LOSE!", 1.0, 410, 440, 30);

    //draw highscores
    for(int i=0; i<10; i++)
    {
        if(simConfig->gameData.highscores[i]!=NULL)
        {
            score* currentScore = simConfig->gameData.highscores[i];//for readability
            sprites.generateText(simConfig, to_string(int(floor(currentScore->currScore))), 0.3, 225, 180-20*i,-2);//draw score
        }
    }
}

bool renderer::initialize()
{
    simConfig->simShaderManager->activateShader("2D");

    // create the fbo
    glGenFramebuffers(1, &fbo_main);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_main);;

	texID = glGetUniformLocation(simConfig->simShaderManager->activeProgram, "gSampler");


	// The texture we're going to render to
	glGenTextures(1, &renderedTexture);
	
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, renderedTexture);

	// Give an empty image to OpenGL ( the last "0" means "empty" )
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, simConfig->getWindowWidth(), simConfig->getWindowHeight(), 0,GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

	// The depth buffer
	if ( !GLEW_ARB_framebuffer_object ){ // OpenGL 2.1 doesn't require this, 3.1+ does
		printf("Your GPU does not provide framebuffer objects. Use a texture instead.");
		return false;
	}
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, simConfig->getWindowWidth(), simConfig->getWindowHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTexture, 0);

	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
cout<<"FUCK YOU"<<endl;

    //create a sprite to render
    outputSprite = new sprite(0, 0, simConfig->getWindowWidth(), simConfig->getWindowHeight(), 1.0, 1.0);
    outputSprite->load(simConfig,"interface/airhockeyinterface.png");
    outputSprite->init(simConfig);

    //enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    sprites.init(simConfig);

    //create static interfaces
    sprites.addSprite(simConfig, 0, 0, 1280, 800, "interface/airhockeyinterface.png", true);

    //and its done
    return true;
}
