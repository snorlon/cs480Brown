#include "entitymanager.h"
#include "entity.cpp"
#include <cstddef>
#include <iostream>
#include <fstream>
#include <sstream>

entityManager::entityManager()
{
    head = NULL;
    entityCount = 0;

    simConfig = NULL;
}

void entityManager::initialize()
{
    entity* iterator = head;

    //init each entity
    while(iterator!=NULL)
    {
        iterator->init();
        iterator = iterator->next;
    }
}

void entityManager::tick(float dt)
{
    entity* iterator = head;

    //tick each entity, let them figure out what they want done
    while(iterator!=NULL)
    {
        iterator->tick(dt);
        iterator = iterator->next;
    }
}

void entityManager::loadData( string loadlistFileName )
{
    string data;

    ifstream input;
    stringstream ss;
    input.open(loadlistFileName);

    input>>data;

    if(!input.good())
    {
        cout<<"Failed to load "<<loadlistFileName<<"!"<<endl;
        return;
    }

    //look up every filename attached to an object we want to load
    while(input.good())
    {

        //create a new entity for our shiny new object
        entity* newObj = loadEntity(data);

        if(newObj!=NULL)
        {
            //add them to the object list
            newObj->next = head;
            head = newObj;
            entityCount++; 
        }       

        input>>data;
    }

    input.close();

    cout<<"Successfully loaded data from "<<loadlistFileName<<"!"<<endl;

}

entity* entityManager::loadEntity(string fileName)
{
    string data;
    string data2;
    string data3;
    entity* newObj = new entity(simConfig);

    //now load each of THOSE files, this time creating an entity based on their data
    ifstream objectFile;
    objectFile.open(fileName);
    int line = 0; //tracks what stage of file loading we are in
    
    objectFile>>data;

    //if it failed to open, skip over this file
    if(!objectFile.good())
    {
        cout<<"Failed to load object! "<<fileName<<endl;
        delete newObj;

        return NULL;
    }

    while(objectFile.good())
    {
        if(line>=8) // all objects past this point should be children
        {
            //create a new entity for our shiny new object
            entity* newEnt = loadEntity(data);
            
            if(newEnt!=NULL)
            {
                //add them to the object list
                newEnt->next = head;
                head = newEnt;
                entityCount++; 

                newEnt->parent = newObj;
            }      
        }
        else
        {
            if(data == "Name:")//name loading
            {
                objectFile>>data2;
                newObj->name = data2;
            }
            else if(data == "Shape:")//shape loading
            {
                objectFile>>data2;
                newObj->shape = data2;
            }
            else if(data == "Scale:")//mass loading
            {
                double newR = 1;
                objectFile>>newR;
                newObj->radius = newR;
            }
            else if(data == "Mass:")//scale loading
            {
                double newR = 1;
                objectFile>>newR;
                newObj->objPhysics.mass = newR;
            }
            else if(data == "Location:")//Initial position loading
            {
                char dummy;
                double newPos = 0;
                objectFile>>newPos;
                newObj->absolutePosition.x = newPos;
                objectFile>>dummy;
                objectFile>>newPos;
                newObj->absolutePosition.y = newPos;
                objectFile>>dummy;
                objectFile>>newPos;
                newObj->absolutePosition.z = newPos;
            }
            else if(data == "Velocity:")//Initial speed loading
            {
                char dummy;
                double newPos = 0;
                objectFile>>newPos;
                newObj->velocity.x = newPos;
                objectFile>>dummy;
                objectFile>>newPos;
                newObj->velocity.y = newPos;
                objectFile>>dummy;
                objectFile>>newPos;
                newObj->velocity.z = newPos;
            }
            else if(data == "Model:")//model loading
            {
                objectFile>>data2;
                objectFile>>data3;
                cout<<"Model file path: "<<data2<<data3<<endl;

                //oh god monster code section//open the file
                const aiScene* scene = import.ReadFile("../bin/models/"+data2+data3+".obj", aiProcess_Triangulate);

                if(scene != NULL)
                {
                    //iterate across the meshes in the scene
                    for(unsigned int mIndex = 0; mIndex < scene->mNumMeshes; mIndex++)
                    {
                        aiMesh* m = scene->mMeshes[mIndex];
                        //iterate across faces in the mesh
                        for(unsigned int fIndex = 0; fIndex < m->mNumFaces; fIndex++)
                        {
                            aiFace* f = &(m->mFaces[fIndex]);
                            //iterate across indices in the face
                            for(unsigned int iIndex = 0; iIndex < f->mNumIndices; iIndex++)
                            {
                                const int index = f->mIndices[iIndex];

                                //push each vertex onto the entity vertices
                                Vertex v;
                                v.position[0] = m->mVertices[index].x;
                                v.position[1] = m->mVertices[index].y;
                                v.position[2] = m->mVertices[index].z;

                                //get the material
                                aiMaterial* material = scene->mMaterials[m->mMaterialIndex];
                                aiColor3D color (0.f,0.f,0.f);
                                material->Get(AI_MATKEY_COLOR_DIFFUSE,color);

                                if(m->HasTextureCoords(0))
                                {

                                    aiVector3D textureCoord = m->mTextureCoords[0][index];

                                    v.uv[0] = textureCoord.x;
                                    v.uv[1] = textureCoord.y;
                                }
                                else
                                {
                                    v.uv[0] = 0.0f;
                                    v.uv[1] = 0.0f;
                                }

                                newObj->vertices.push_back(v);
                            }
                        }

                        //get texture file name
                        aiString fname;
                        char newname[512];
                        string textPath = "../bin/models/" + data2;

                        strcpy(newname, textPath.c_str());
                        
                        scene->mMaterials[m->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &fname);

                        //add mat name to end of it
                        strcat(newname, fname.data);

                        //give it its texture
                        newObj->texID = simConfig->texCount;
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
                        {
                            cout<<"Dib failed to load! Are your file paths set up properly?? "<<newname<<endl;
                        }

                        bits = FreeImage_GetBits(dib);
                        //get the image width and height
                        newObj->texWidth = FreeImage_GetWidth(dib);
                        newObj->texHeight = FreeImage_GetHeight(dib);
                        cout<<"Texture "<<newname<<" image size: "<<newObj->texWidth<<"px by "<<newObj->texHeight<<"px"<<endl;
                        //if this somehow one of these failed (they shouldn't), return failure

                        //generate an OpenGL texture ID for this texture
                        glGenTextures(1, &newObj->texID);
                        glBindTexture( GL_TEXTURE_2D, newObj->texID);
                        //store the texture data for OpenGL use
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newObj->texWidth, newObj->texHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, bits);

                        FreeImage_Unload(dib);
                    }
                }
                else
                    cout<<"Failed to create scene!"<<endl;
            }
            /* REMOVED FOR PRESSING CEREMONIAL REASONS
            switch(line)
            {
                case 9://check if we need a camera of this object
                    if(atoi(data.c_str()) == 1)
                    {
                        Camera* newCamera = new Camera(newObj);
                        if(simConfig->presetCameras == NULL)
                            simConfig->presetCameras = newCamera;
                        else
                        {
                            Camera* iterator = simConfig->presetCameras;
                            while(iterator->next!=NULL)
                                iterator = iterator->next;

                            iterator->next = newCamera;
                        }
                        cout<<"New camera created for "<<newObj->name<<"!"<<endl;
                    }
                    break;
            }*/
        }

        line++;

        objectFile>>data;
    }
    objectFile.close();

    return newObj;
}


void entityManager::render()
{
    entity* iterator = head;

    //draw each entity, let them figure out what they want done
    while(iterator!=NULL)
    {
        iterator->render();
        iterator = iterator->next;
    }
}

void entityManager::cleanup()
{
    //kill each entity one by one
    while(head!=NULL)
    {
        entity* temp = head;
        head = head->next;

        temp->cleanup();

        delete temp;
    }
}
