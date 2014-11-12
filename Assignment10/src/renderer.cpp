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
    //glBindFramebuffer(GL_FRAMEBUFFER, fbo_main);
    //clear the screen
    glClearColor(0.1, 0.1, 0.15, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //activate 3D shader
    simConfig->simShaderManager->activate3DShaders(false);

    //perform pre-rendering
    glUseProgram(simConfig->program);
    simConfig->simEntityManager->prerender();

    glUseProgram(simConfig->program);

    //throw in entity manager rendering
    simConfig->simEntityManager->render();

    //render game objects
    simConfig->gameData.render();

    //
    //attempt the texture map display
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    simConfig->simShaderManager->activate3DShaders(true);
    // Render to the screen

                // Clear the screen
                //glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // Use our shader

                // Bind our texture in Texture Unit 0
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, colorBuffer);
                // Set our "renderedTexture" sampler to user Texture Unit 0
                //glUniform1i(texID, 0);

                //outputSprite->render();



    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //activate 2D shader
    simConfig->simShaderManager->activate2DShaders();

    //render the 2d images
    sprites.render(  );
                           
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
    sprites.generateText(simConfig, simConfig->gameData.currentGame.p1Name, 0.5, 330, 740);
    sprites.generateText(simConfig, ""+to_string(simConfig->gameData.currentGame.score1), 0.5, 525, 740);
    sprites.generateText(simConfig, simConfig->gameData.currentGame.p2Name, 0.5, 770, 740);
    sprites.generateText(simConfig, ""+to_string(simConfig->gameData.currentGame.score2), 0.5, 700, 740);

    //ai flags
    if(simConfig->gameData.ai1Enabled)
        sprites.generateText(simConfig, "AI ENABLED", 0.3, 370, 700,-2);
    if(simConfig->gameData.ai2Enabled)
        sprites.generateText(simConfig, "AI ENABLED", 0.3, 740, 700,-2);

    //render GAMEOVER if game is not active
    if(!simConfig->gameData.gameActive)
        sprites.generateText(simConfig, "GAME OVER!", 1.0, 410, 440, 30);

    //render our theme name
    if(simConfig->gameData.currentTheme!=NULL)
    {
        sprites.generateText(simConfig, "Theme: ", 0.6, 365, 138,7);
        sprites.generateText(simConfig, simConfig->gameData.currentTheme->themeName, 0.6, 600, 138,7);//draw theme name
    }

    //draw highscores
    for(int i=0; i<10; i++)
    {
        if(simConfig->gameData.highscores[i]!=NULL)
        {
            score* currentScore = simConfig->gameData.highscores[i];//for readability
            sprites.generateText(simConfig, currentScore->bestName(), 0.3, 45, 180-20*i,-2);//draw name
            sprites.generateText(simConfig, to_string(currentScore->bestScore()), 0.3, 225, 180-20*i,-2);//draw score
        }
    }

    //render theme images
    sprites.addSprite(simConfig, 316, 0, 648, 131, "interface/themes/"+simConfig->gameData.currentTheme->themeName+".png", false);
}

bool renderer::initialize()
{
    // create the color texture
    glGenTextures(1, &colorBuffer);
    glBindTexture(GL_TEXTURE_2D, colorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 768, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // create the depth texture
    glGenTextures(1, &depthBuffer);
    glBindTexture(GL_TEXTURE_2D, depthBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 768, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // create the fbo
    glGenFramebuffers(1, &fbo_main);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_main);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //create a sprite to render
    outputSprite = new sprite(0, 0, 1024, 768, 1.0, 1.0);
    outputSprite->init(simConfig);

    texID = glGetUniformLocation(simConfig->program, "colorBuffer");
    outputSprite->loc_texture = texID;

    //enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    sprites.init(simConfig);

    //create static interfaces
    sprites.addSprite(simConfig, 0, 0, 1280, 800, "interface/airhockeyinterface.png", true);

    //and its done
    return true;
}
