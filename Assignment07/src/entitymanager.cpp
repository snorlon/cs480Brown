#include "entitymanager.h"
#include "entity.cpp"
#include <cstddef>
#include <iostream>
#include <fstream>

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
        //now load each of THOSE files, this time creating an entity based on their data
        ifstream objectFile;
        objectFile.open(data);
        int line = 0; //tracks what stage of file loading we are in
        
        //objectFile>>data;
        getline(objectFile, data);

        //if it failed to open, skip over this file
        if(!objectFile.good())
        {
            cout<<"Failed to load object!"<<endl;
            continue;
        }

        //create a new entity for our shiny new object
        entity* newObj = new entity(simConfig);

        //add them to the object list, we don't care if they're visible - yet
        newObj->next = head;//add entity to list
        head = newObj;
        head->semimajorAxis = entityCount;
        entityCount++;

        while(objectFile.good())
        {
            switch(line)
            {
                case 0:
                    newObj->name = data;
                    cout<<"Object name: "<<data<<endl;
                    break;
                case 1:
                    cout<<"Model filename: "<<data<<endl;

                    //oh god monster code section//open the file
                    const aiScene* scene = import.ReadFile("../bin/models/"+data+newObj->name+".obj", aiProcess_Triangulate);

                    if(scene != NULL)
                    {

                        //iterate across the meshes in the scene
                        for(unsigned int mIndex = 0; mIndex < scene->mNumMeshes; mIndex++)
                        {
                            const aiMesh* m = scene->mMeshes[mIndex];
                            //iterate across faces in the mesh
                            for(unsigned int fIndex = 0; fIndex < m->mNumFaces; fIndex++)
                            {
                                const aiFace* f = &(m->mFaces[fIndex]);
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
                                    const aiMaterial* material = scene->mMaterials[m->mMaterialIndex];
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
                            string textPath = "../bin/models/" + data;

                            strcpy(newname, textPath.c_str());
                            
                            scene->mMaterials[m->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &fname);

                            //add mat name to end of it
                            strcat(newname, fname.C_Str());

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
                            //if still unkown, return failure
                            if(fif == FIF_UNKNOWN)
                                return;

                            if(FreeImage_FIFSupportsReading(fif))
                                dib = FreeImage_Load(fif, newname, 0);
                            else
                                cout<<"Bad texture file format!"<<endl;

                            if(!dib)
                            {
                                cout<<"Dib failed to load! Are your file paths set up properly?? "<<newname<<endl;
                                return;
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
                    break;
            }

            line++;

            objectFile>>data;
        }

        input>>data;
    }

    cout<<"Successfully loaded data from "<<loadlistFileName<<"!"<<endl;

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
        delete temp;
    }
}
