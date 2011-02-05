#ifndef H_INCLUDES
#define H_INCLUDES

using namespace std;

#include "platformIncludes.h"

//FIFO and string
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <typeinfo>
#include <string.h>


//data structures
#include <math.h>

#include <vector>

#include <map>
#include <algorithm>


//external user libraries
#include "vmath.h"


#include "tinyxml.h"
#include "tinystr.h"

#include "ode.h"

#include "FreeImage.h"


///Structs and global stuff!


class Actor;

struct actorID
       {
       Actor* actorReference;
       const std::type_info* actorType;
       int actorSize;
       };


//TODO: maybe make toString function in here?
struct memberID
       {
       void* memberReference;
       const std::type_info* memberType;
       int memberSize;
       char* memberName;
       };



struct textureObject{

    GLuint texture;
    bool bAlpha;
    bool bWrap;
    std::string nextTexture;
    std::string texFilename;
    float frameRate;
};

struct shaderObject{

    GLuint shader;
    std::string vertexShaderFilename;
    std::string fragmentShaderFilename;
};


struct bone{

    Matrix4f* boneMatrix;
    Matrix4f* invBoneMatrix;
    std::string name;
    bone*      parentBone;
};

struct vertexData{

    Vector4f location;
    Vector4f color;
    Vector3f secondaryColor;
    Vector3f normal;
    Vector3f texCoord;
    Vector4f boneReferences;
    Vector4f vertexWeights;
    float vertexID;
    float birth;

};

//for keyframing and stuff...

struct key{
    Vector3f    locationKey;                        //stores location Key
    Vector3f    rotationKey;                        //stores rotation Key
    Vector3f    scaleKey;                           //stores scale Key
    std::map<std::string,Matrix4f> boneMatrices;    //stores Matrices to all bones in a skeletalActor
    double       timeKey;                           //the key's time
    };


// pound defines

#ifdef TARGET_WIN32
#define DIRECTORY_SEPARATION "\\"
#endif

#ifdef TARGET_MACOSX
#define DIRECTORY_SEPARATION "/"
#endif


#define COLOR_WHITE     Vector4f(1,1,1,1);
#define COLOR_YELLOW    Vector4f(1,1,0,1);
#define COLOR_BLACK     Vector4f(0,0,0,1);
#define COLOR_RED       Vector4f(1,0,0,1);
#define COLOR_GREEN     Vector4f(0,1,0,1);
#define COLOR_BLUE      Vector4f(0,0,1,1);


#endif
