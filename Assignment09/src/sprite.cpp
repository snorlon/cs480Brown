#include "sprite.h"
#include "config.h"

//start sprite data members
sprite::sprite(config* simConfig, int nx, int ny, int nwidth, int nheight, string fpath, double nscaleX, double nscaleY)
{
    next = NULL;
    children = NULL;
    
    width = nwidth;
    height = nheight;
    x = nx;
    y = ny;
    scaleX = nscaleX*2;
    scaleY = nscaleY*2;

    double xOffset = -simConfig->getWindowWidth();
    double yOffset = -simConfig->getWindowHeight();

    double w = simConfig->getWindowWidth();
    double h = simConfig->getWindowHeight();

    //generate our shape
    Vertex v1;
    v1.position[0] = (xOffset+x*scaleX)/w;
    v1.position[1] = (yOffset+y*scaleY)/h;

    v1.uv[0] = 0;
    v1.uv[1] = 0;

    Vertex v2;
    v2.position[0] = (xOffset+(x+width)*scaleX)/w;
    v2.position[1] = (yOffset+y*scaleY)/h;

    v2.uv[0] = 1;
    v2.uv[1] = 0;

    Vertex v3;
    v3.position[0] = (xOffset+x*scaleX)/w;
    v3.position[1] = (yOffset+(y+height)*scaleY)/h;

    v3.uv[0] = 0;
    v3.uv[1] = 1;

    Vertex v4;
    v4.position[0] = (xOffset+(x+width)*scaleX)/w;
    v4.position[1] = (yOffset+(y+height)*scaleY)/h;

    v4.uv[0] = 1;
    v4.uv[1] = 1;


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

    //generate texture data
    //get texture file name
    char newname[512];
    string textPath = "../bin/sprites/" + fpath;

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
    {
        cout<<"WE DON'T KNOW WHAT FIF THIS IS!"<<endl;
    }

    if(FreeImage_FIFSupportsReading(fif))
        dib = FreeImage_Load(fif, newname, 0);
    else
    {
        cout<<"Bad texture file format!"<<endl;
    }

    if(!dib)
    {
        cout<<"Dib failed to load! Are your file paths set up properly?? "<<newname<<endl;
    }

    bits = FreeImage_GetBits(dib);
    //get the image width and height
    texWidth = FreeImage_GetWidth(dib);
    texHeight = FreeImage_GetHeight(dib);
    cout<<"Texture "<<newname<<" image size: "<<texWidth<<"px by "<<texHeight<<"px"<<endl;

    //generate an OpenGL texture ID for this texture
    glGenTextures(1, &texID);
    glBindTexture( GL_TEXTURE_2D, texID);
    //store the texture data for OpenGL use
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, bits);

    FreeImage_Unload(dib);



    //unbind buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

sprite::~sprite()
{
    while(children!=NULL)
    {
        sprite* temp = children;
        children = children->next;
        delete temp;
    }

    glDeleteBuffers(1, &vbo_sprite);
}

void sprite::render( config* simConfig )
{
    simConfig = NULL;//unused to shutup warning
    simConfig = simConfig;

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_sprite);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);

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

    //glEnable(GL_DEPTH_TEST);

    //unbind buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisable (GL_BLEND);
}


//start sprite manager data members
spriteManager::spriteManager()
{
    children = NULL;
}

spriteManager::~spriteManager()
{
    while(children!=NULL)
    {
        sprite* temp = children;
        children = children->next;
        delete temp;
    }
}

void spriteManager::render( config* simConfig )
{ 

    sprite* iterator = children;
    while(iterator!=NULL)
    {
        iterator->render(simConfig);
        iterator = iterator->next;
    }
}

void spriteManager::addSprite(config* simConfig, int nx, int ny, int nwidth, int nheight, string fpath, double nscaleX, double nscaleY)
{
    simConfig->simShaderManager->activate2DShaders();
    sprite* newSprite = new sprite(simConfig, nx, ny, nwidth, nheight, fpath, nscaleX, nscaleY);

    sprite* iterator = children;
    if(children == NULL)
        children = newSprite;
    else
    {
        while(iterator->next != NULL)
            iterator = iterator->next;

        iterator->next = newSprite;
    }
}
