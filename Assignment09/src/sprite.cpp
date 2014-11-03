#include "sprite.h"
#include "config.h"

//start sprite data members
sprite::sprite(int nx, int ny, int nwidth, int nheight, double nscaleX, double nscaleY)
{
    next = NULL;
    
    width = nwidth;
    height = nheight;
    x = nx;
    y = ny;
    scaleX = nscaleX*2;
    scaleY = nscaleY*2;

    texWidth = 1;
    texHeight = 1;

    frameSize[0] = 1;
    frameSize[1] = 1;

    frame[0] = 0;
    frame[1] = 0;

    isFont = true;
}

sprite::~sprite()
{
    glDeleteBuffers(1, &vbo_sprite);
    if(!isFont)
        glDeleteTextures(1, &texID);
}

void sprite::rebuild(config* simConfig)
{
    glDeleteBuffers(1, &vbo_sprite);

    init(simConfig);//then reinit ourself
}

void sprite::init(config* simConfig)
{
    //clear the vertices
    vertices.clear();

    double xOffset = -1280;
    double yOffset = -800;

    double w = 1280;
    double h = 800;

    //generate our shape
    Vertex v1;
    v1.position[0] = (xOffset+x)/w;
    v1.position[1] = (yOffset+y)/h;

    v1.uv[0] = frame[0]*frameSize[0]/texWidth;
    v1.uv[1] = 1.0-(frame[1]+1)*frameSize[1]/texHeight;

    Vertex v2;
    v2.position[0] = (xOffset+x+width*scaleX)/w;
    v2.position[1] = (yOffset+y)/h;

    v2.uv[0] = (frame[0]+1)*(frameSize[0]/texWidth);
    v2.uv[1] = 1.0-(frame[1]+1)*frameSize[1]/texHeight;

    Vertex v3;
    v3.position[0] = (xOffset+x)/w;
    v3.position[1] = (yOffset+y+height*scaleY)/h;

    v3.uv[0] = frame[0]*frameSize[0]/texWidth;
    v3.uv[1] = 1.0-frame[1]*frameSize[1]/texHeight;

    Vertex v4;
    v4.position[0] = (xOffset+x+width*scaleX)/w;
    v4.position[1] = (yOffset+y+height*scaleY)/h;

    v4.uv[0] = (frame[0]+1)*frameSize[0]/texWidth;
    v4.uv[1] = 1.0-frame[1]*frameSize[1]/texHeight;


    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v4);
    vertices.push_back(v3);

    glGenBuffers(1, &vbo_sprite);

    //temporary generation here
    glBindBuffer(GL_ARRAY_BUFFER, vbo_sprite);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    loc_position = glGetAttribLocation(simConfig->program, "s_position");
    loc_texture = glGetAttribLocation(simConfig->program, "s_tex");
}

void sprite::setFrameData( int frameWidth, int frameHeight )
{
    frameSize[0] = frameWidth;
    frameSize[1] = frameHeight;
}

void sprite::setFrame( int newFrameX, int newFrameY )
{
    frame[0] = newFrameX;
    frame[1] = newFrameY;
}

void sprite::load(config* simConfig, string fpath)
{
    //generate texture data
    //get texture file name
    char newname[512];
    string textPath = "../bin/sprites/" + fpath;

    isFont = false;

    strcpy(newname, textPath.c_str());

    //give it its texture
    texID = simConfig->texCount;
    simConfig->texCount++;

    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

    BYTE * bits(0);
    FIBITMAP * dib(0);
    fif = FreeImage_GetFileType(newname, 0);
    //if still unknown, try to guess the file format from the file extension
    if(fif == FIF_UNKNOWN)
        fif = FreeImage_GetFIFFromFilename(newname);

    if(fif == FIF_UNKNOWN)
        cout<<"WE DON'T KNOW WHAT FIF THIS IS!"<<endl;

    if(FreeImage_FIFSupportsReading(fif))
        dib = FreeImage_Load(fif, newname, 0);
    else
        cout<<"Bad texture file format!"<<endl;

    if(!dib)
        cout<<"Dib failed to load! Are your file paths set up properly?? "<<newname<<endl;

    bits = FreeImage_GetBits(dib);
    //get the image width and height
    texWidth = FreeImage_GetWidth(dib);
    texHeight = FreeImage_GetHeight(dib);

    //generate an OpenGL texture ID for this texture
    glGenTextures(1, &texID);
    glBindTexture( GL_TEXTURE_2D, texID);
    //store the texture data for OpenGL use
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, bits);

    FreeImage_Unload(dib);

    //reset texture frame sizes to max
    frameSize[0] = texWidth;
    frameSize[1] = texHeight;
}

void sprite::render(  )
{
    glEnable (GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_sprite);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);

    //set up the Vertex Buffer Object so it can be drawn
    glEnableVertexAttribArray(loc_position);
    //set pointers into the vbo for each of the attributes(position and color)
    glVertexAttribPointer( loc_position,//location of attribute
                           3,//number of elements
                           GL_FLOAT,//type
                           GL_FALSE,//normalized?
                           sizeof(Vertex),//stride
                           0);//offset

    glEnableVertexAttribArray(loc_texture);
    glVertexAttribPointer( loc_texture,
                           2,
                           GL_FLOAT,
                           GL_FALSE,
                           sizeof(Vertex),
                            (void*)offsetof(Vertex,uv));

    glDrawArrays(GL_QUADS, 0, 4);//mode, starting index, count

    //clean up
    glDisableVertexAttribArray(loc_position);
    glDisableVertexAttribArray(loc_texture);

    //unbind buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisable (GL_BLEND);
}


//start sprite manager data members
spriteManager::spriteManager()
{
    staticSprites = NULL;
    dynamicSprites = NULL;

    font = NULL;
}

spriteManager::~spriteManager()
{
    clearDynamic();
    while(staticSprites!=NULL)
    {
        sprite* temp = staticSprites;
        staticSprites = staticSprites->next;
        delete temp;
    }
}

void spriteManager::clearDynamic()
{
    while(dynamicSprites!=NULL)
    {
        sprite* temp = dynamicSprites;
        dynamicSprites = dynamicSprites->next;
        delete temp;
    }
}

void spriteManager::render(  )
{ 
    //for our engine, by default render static first, we don't need a proper layering system

    sprite* iterator2 = dynamicSprites;
    while(iterator2!=NULL)
    {
        iterator2->render();
        iterator2 = iterator2->next;
    }
    sprite* iterator = staticSprites;
    while(iterator!=NULL)
    {
        iterator->render();
        iterator = iterator->next;
    }
}

void spriteManager::init(config* simConfig)
{
    //load in our font
    font = new sprite(0, 0, 10, 10, 1.0, 1.0);
    font->load(simConfig, "interface/font.png"); //load the texture
    font->init(simConfig);
}

sprite* spriteManager::addSprite(config* simConfig, int nx, int ny, int nwidth, int nheight, string fpath, bool isStatic, double nscaleX, double nscaleY, bool useFont)
{
    simConfig->simShaderManager->activate2DShaders();
    sprite* newSprite = new sprite(nx*2, ny*2, nwidth, nheight, nscaleX, nscaleY);

    if(isStatic)
    {
        if(staticSprites == NULL)
        {
            staticSprites = newSprite;
        }
        else
        {
            sprite* iterator = staticSprites;
            while(iterator->next != NULL)
                iterator = iterator->next;

            iterator->next = newSprite;
        }
    }
    else
    {
        if(dynamicSprites == NULL)
        {
            dynamicSprites = newSprite;
        }
        else
        {
            sprite* iterator = dynamicSprites;
            while(iterator->next != NULL)
                iterator = iterator->next;

            iterator->next = newSprite;
        }
    }

    if(!useFont)
        newSprite->load(simConfig, fpath); //load the texture
    else
    {
        newSprite->texID = font->texID;
        newSprite->texHeight = font->texHeight;
        newSprite->texWidth = font->texWidth;
        newSprite->setFrameData(72,80);
    }

    newSprite->init(simConfig);

    return newSprite;//return our new sprite for operations
}

void spriteManager::generateText(config* simConfig, string text, double scale, int startX, int startY, double gapOverride)
{
    double xPos = startX;
    double yPos = startY;
//72x80
//45x45

    for(unsigned int i=0; i<text.length(); i++)
    {
        //only draw legal characters
        if((text[i]<='9'&&text[i]>='0')||(text[i]<='z'&&text[i]>='a')||(text[i]<='Z'&&text[i]>='A')||text[i]=='\''||text[i]=='"'||text[i]=='!'||text[i]=='.'||text[i]=='?')
        {
            sprite* letter = addSprite(simConfig, xPos, yPos, 72, 80, "interface/font.png", false, scale, scale, true);
            //set frame according to character
            if(text[i]<='M' && text[i]>='A')
                letter->setFrame( text[i]-65, 0 );
            else if(text[i]<='m' && text[i]>='a')
                letter->setFrame( text[i]-97, 0 );
            else if(text[i]<='Z' && text[i]>='N')
                letter->setFrame( text[i]-78, 1 );
            else if(text[i]<='z' && text[i]>='n')
                letter->setFrame( text[i]-110, 1 );
            else if(text[i]<='9' && text[i]>='0')
                letter->setFrame( text[i]-48, 2 );
            else if(text[i]=='.')
                letter->setFrame( 0, 3 );
            else if(text[i]=='!')
                letter->setFrame( 7, 3 );
            else if(text[i]=='\'')
                letter->setFrame( 6, 3 );
            else if(text[i]=='"')
                letter->setFrame( 8, 3 );
            else if(text[i]=='?')
                letter->setFrame( 10, 3 );
            letter->rebuild(simConfig);
        }
        xPos+=18;
        xPos+=gapOverride;
    }
}
