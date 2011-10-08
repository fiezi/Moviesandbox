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

//removed for now...
//#include "ode.h"

#include "FreeImage.h"


///Structs and global stuff!


class Actor;

struct actorID
       {
       Actor* actorReference;
       const std::type_info* actorType;
       int actorSize;
       };


//keep reference to propertyButton in here,
//to store the best way of changing its values
struct memberID
       {
       void* memberReference;
       const std::type_info* memberType;
       int memberSize;
       char* memberName;
       //these are for interface reasons
       std::string propertyButtonType;
       bool bShowProperty;
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
    map<string,int> uniforms;
};


struct bone{

    Matrix4f* boneMatrix;
    Matrix4f* invBoneMatrix;
    std::string name;
    bone*      parentBone;
};


//TODO: remove normals from vertexData?
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

//handles starting and stopping of external processes
struct externalInputData{

        std::string taskName;
        std::string filename;

#ifdef TARGET_WIN32

        PROCESS_INFORMATION processId;

        void startProgram(){

            STARTUPINFO si;

            char* exePath=(char*)filename.c_str();
            size_t pos;
            pos=filename.find_last_of("/\\");
            string workDir=filename.substr (0,pos);
            //TODO - create workingDir
            char* workingDir=(char*)workDir.c_str();


            ZeroMemory( &si, sizeof(si) );
            si.cb = sizeof(si);
            ZeroMemory( &processId, sizeof(processId) );

            // Start the child process.
            if( !CreateProcess( NULL,   // No module name (use command line)
                exePath,        // Command line
                NULL,           // Process handle not inheritable
                NULL,           // Thread handle not inheritable
                FALSE,          // Set handle inheritance to FALSE
                0,              // No creation flags
                NULL,           // Use parent's environment block
                workingDir,           // Use working directory
                &si,            // Pointer to STARTUPINFO structure
                &processId )           // Pointer to PROCESS_INFORMATION structure
            )
            {
                printf( "CreateProcess failed (%d).\n", (int)GetLastError() );
                return;
            }

            cout << "started task "<<taskName<<endl;
        }

        void stopProgram(){
            cout << "closing task "<< taskName << endl;

            TerminateProcess(processId.hProcess,0);
            CloseHandle( processId.hProcess );
            CloseHandle( processId.hThread );
        }

#endif
#ifdef TARGET_MACOSX

        pid_t processId;

        void startProgram(){

            if ((processId = fork()) == 0) {
                char app[] = "tools/msbKinect/msbKinect.app/Contents/MacOS/msbKinect";
                char * const argv[] = { app, NULL };
                if (execve(app, argv, NULL) < 0) {
                    perror("execv error:");
                }
            } else if (processId < 0) {
                perror("fork error");
            }
            cout << "started task "<<taskName<<endl;
        }

        void stopProgram(){
            //taken from: http://www.yolinux.com/TUTORIALS/ForkExecProcesses.html

            int  killReturn = killpg( processId, SIGKILL);  // Kill child process group

            if( killReturn == ESRCH)      // pid does not exist
            {
               cout << "Group does not exist!" << endl;
            }
            else if( killReturn == EPERM) // No permission to send signal
            {
               cout << "No permission to send signal!" << endl;
            }
            else
               cout << "closing task " << taskName << endl;
        }
#endif
#ifdef TARGET_LINUX

        pid_t processId;

        void startProgram(){

            if ((processId = fork()) == 0) {
                char app[] = "tools/msbKinect/msbKinect";
                char * const argv[] = { app, NULL };
                if (execve(app, argv, NULL) < 0) {
                    perror("execv error:");
                }
            } else if (processId < 0) {
                perror("fork error");
            }
            cout << "started task "<<taskName<<endl;
        }

        void stopProgram(){
            //taken from: http://www.yolinux.com/TUTORIALS/ForkExecProcesses.html

            int  killReturn = killpg( processId, SIGKILL);  // Kill child process group

            if( killReturn == ESRCH)      // pid does not exist
            {
               cout << "Group does not exist!" << endl;
            }
            else if( killReturn == EPERM) // No permission to send signal
            {
               cout << "No permission to send signal!" << endl;
            }
            else
               cout << "closing task " << taskName << endl;
        }
#endif

};
//for keyframing and stuff...

struct key{
    Matrix4f    transformKey;                        //stores location Key
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

#ifdef TARGET_LINUX
#define DIRECTORY_SEPARATION "/"
#endif

//TODO: let's tone down the contrast a bit, shall we?
#define COLOR_WHITE Vector4f(0.8,0.8,0.8,1);
#define COLOR_YELLOW Vector4f(0.8,0.8,0,1);
#define COLOR_BLACK Vector4f(0.1,0.1,0.1,1);
#define COLOR_RED Vector4f(0.8,0,0,1);
#define COLOR_GREEN Vector4f(0,0.8,0,1);
#define COLOR_BLUE Vector4f(0,0,0.8,1);


#endif
