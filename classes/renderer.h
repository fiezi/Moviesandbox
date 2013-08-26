#ifndef H_RENDERER
#define H_RENDERER

#include "msbObject.h"
#include "sceneData.h"
#include "content.h"
#include "brush.h"
#include "layer.h"
#include "brush.h"
#include "particleSystem.h"
#include "basicButton.h"
#include "node.h"
#include "action.h"
#include "textfile.h"

#include "msbLight.h"

#include "ofTrueTypeFont.h"



#define BUFSIZE 512
#define FPSBUFFERSIZE 10


using namespace std;

class MeshData;

class Renderer: public MsbObject{

public:

        //verion debug

       static Renderer* rendererInstance;
       Input*           input;
       SceneData*       sceneData;
       GLuint selectBuf[BUFSIZE];          //for picking

       std::string  startSceneFilename;    //basic scene
       vector<std::string>  library;       //all libraries

       string currentShader;                //currently bound shader
       string lastShader;

       bool bRenderStereo,                  //render stereoscpic into Framebuffers
            bDrawLighting,                  //include Lights and Shadows in Rendering
            bDrawNormals,                   //draw screen space normals for debugging
            bDrawSmudge,                  //include Lights and Shadows in Rendering
            bDrawNodes,                     //draw nodes
            bDrawMenu,                      //show menu in 3D renderer
            bUseBlending,
            bMultisample,                   //Multisample FBOs
            bSSAO,                          //draw screen-space ambient occlusion
            bDOF,                           //draw Depth of Field
            bDrawColor,                 	//draw basic Color image (would be lighting only if false)
            bShadowPass;                    //currently rendering shadows

        //TODO: SPECIAL BALDA
        bool bInvert;                        //invert colors

       int numSamples;                      //number of multisamples

       //Physics using ODE
       /* - disabled for now
       dWorldID physicsWorld;              // our physics world
       dSpaceID collisionSpace;            // using ODE collision
       dJointGroupID jointGroup;           // a joint group
       dGeomID groundPlane;                // our ground plane
       */
       bool bUpdatePhysics;                // call physicsUpdate or not
       bool bDebug;                        //Debug mode, will check for OpenGL Errors

       //al and Screen stuff
       bool bFullscreen;
       float nearClip, farClip;            //clipping planes

       float frustumTop,frustumBottom;      //viewFrustum for 3D stuff
       float eyeDistance;                   //eyeDistance for 3D rendering
       float bkgOffset;                     //eyeDistance for 3D rendering

       float focus;                         //camera focus distance

        int normalBlur;                    //smoothing of normals
        int dofBlur;                            //blurriness of DOF

       int      screenX,                     //x-resolution of 3D rendered area
                screenY,                        //y-resolution of 3D rendered area
				windowPosX,                   // OS window position
				windowPosY,                  // OS window position
				windowX,                        //window width
				windowY,                        //window height
                fov;                                  //field of view

       int frames;                         //amount of frames rendered

       double      fpsBuffer[FPSBUFFERSIZE];

       double currentTime,                 //time since program start,
              deltaTime,                   //time since last frame
              physicsTime;                 //time for physics step (sometimes deltaTime is too small to advance physics...)

       Vector3f lightLoc;                  //light Location
       Vector3f ambient;                   //ambient Light

        //depth precision
        GLenum depthPrecision;      //amount of bits for depth operations - usually GL_RGBA8 or GL_RGBA16

        //texture filtering
        GLenum texFilterMin;                    // filtering for procedural textures, usually GL_NEAREST or GL_LINEAR
        GLenum texFilterMag;                    // filtering for procedural textures, usually GL_NEAREST or GL_LINEAR
        GLenum texTGAFilterMin;                   //min filter for file Textures
        GLenum texTGAFilterMag;                   //max filter for file Textures

        //FBO data format
        GLenum dataType;                   //usually GL_INTEGER or GL_FLOAT

        //picking
        GLuint pickTexture;

        //Frame Buffer Objects for light and shadow rendering

        GLuint  lighting_tx;                 // the lightTexture we read from
        GLuint  lighting_fb;                 // the lightFBO we bind
        float     lighting_size;                //resolution for the lighting pass

        GLuint  normal_tx;                 // the lightTexture we read from
        GLuint  normal_fb;                 // the lightFBO we bind
        float    normal_size;                //resolution for the lighting pass

        GLuint  normalBlur_tx;                 // the lightTexture we read from
        GLuint  normalBlur_fb;                 // the lightFBO we bind


        GLuint  shadow_tx;                   // the shadowTexture we read from
        GLuint  shadow_fb;                   // the shadowFBO we bind
        float    shadow_size;

        //Frame Buffer Object for Depth Rendering
        GLuint  depth_tx;
        GLuint  depth_fb;


        GLuint  scene_tx;
        GLuint  scene_fb;
        float    scene_size;

        //Frame Buffer Object for Right Eye Rendering

        GLuint leftEye_tx;
        GLuint leftEye_fb;

        GLuint leftEyeDepth_tx;
        GLuint leftEyeDepth_fb;

        GLuint rightEye_tx;
        GLuint rightEye_fb;

        GLuint rightEyeDepth_tx;
        GLuint rightEyeDepth_fb;

        GLuint multiSample_fb;                      //multisample framebuffer - this is the main FBO we render into
        GLuint multiSample_db;                      //multisample depthbuffer
        GLuint multiSample_color;                   //multisample color texture - the main scene color tex we blit from
        GLuint multiSample_depth;                   //multisample depth texture - normals and depth we blit from
        GLuint multiSample_pick;                    //multisample pick texture - absolute coordinates and objectID we blit from
		GLuint multiSample_lightData;               //multisample fx texture - used for vertexID and other stuff

        //shared memory
        GLuint smTexture;                           //texture written from shared memory

		GLenum drawBuffers[2];

        BasicButton* leftEyeFBO;                 //2D planes for left and right eye!
        BasicButton* rightEyeFBO;
        BasicButton* postOverlay;

        //Matrices
        Matrix4f projectionMatrix;
        Matrix4f inverseProjectionMatrix;
        Matrix4f cameraMatrix;

        Matrix4f cameraRotationMatrix;
        Matrix4f inverseCameraRotationMatrix;

        Matrix4f inverseCameraMatrix;

        Matrix4f cameraProjectionMatrix;
        Matrix4f cameraViewMatrix;
        Matrix4f camViewProjMatrix;

        Matrix4f lightViewMatrix;
        Matrix4f lightViewMatrixInverse;
        Matrix4f lightProjectionMatrix;
        Matrix4f lightProjectionMatrixInverse;
        Matrix4f lightViewProjMatrix;

        ofTrueTypeFont*   verdana;


       Renderer();
       virtual ~Renderer();

       //singleton-ish
       static Renderer* getInstance();

       virtual void registerProperties();

       //***************************************
       //this is stuff we can update easily
       //***************************************
       virtual void initWindow(int x, int y, string windowName);  //
       virtual void reDrawScreen(int w, int h);

#ifdef TARGET_WIN32
       static bool ModifyWindowStyle(HWND hWnd, DWORD dwAdd, DWORD dwRemove, BOOL bEx);
#endif
       virtual void setup();                      //first thing called when program starts
       virtual void physicsSetup();                      //setup ODE

       virtual void update(float deltaTime);
       virtual void physicsUpdate();

       virtual void setupFBOs();

       virtual void createFBO(GLuint* fbObject, GLuint* fbTexture, GLuint* fbDepth, int fbSizeX, int fbSizeY, bool bDepth, string name);
       virtual void checkFBOStatus();

       virtual void draw();
       virtual void drawBackground();
       virtual void drawSceneTexture();
       virtual void draw3D(Layer* currentLayer);
       virtual void draw2D();
       virtual void draw3DOverlay();

       virtual void drawActor(Actor* a);
       virtual void drawButton(BasicButton* b);

       virtual void drawShadows(MsbLight* myLight);
       virtual void drawNormals(Layer* layer);
       virtual void drawDeferredLighting(Layer* layer);

       virtual void performShader(Layer* layer, string sourceTextureID, string destinationTextureID, GLuint renderFBO, string shaderName);

       virtual void displayDebug();

       virtual void drawBone(float width, float height, float depth);
       virtual void drawCube(float scale);
       virtual void drawBoundingBox(Vector3f lowerLeftBack, Vector3f upperRightFront, Vector4f color);
       virtual void drawPlane(float x1,float  y1,float  x2,float  y2, Vector4f color = Vector4f(1.0,1.0,1.0,1.0), bool bCentered = false);
       virtual void drawLine(Vector3f start,Vector3f end, Vector4f startColor=Vector4f(0,0,0,1), Vector4f endColor=Vector4f(0,0,0,1));
       virtual void drawPatch(float width, float height, float resolution);
       virtual void drawGizmos(Actor* a);
       virtual void drawSprite();
       virtual void drawText(string str, float x, float y);

       virtual void drawParticles (Actor* a);
       virtual void drawColladaMesh (Actor* a);
       virtual void drawTriangleMesh(Actor* a);


       virtual void createKinectWall( float resolution );


       virtual void setupCamera(bool bCalculateMatrices);
       virtual void setupOrthoCamera(int width, int height);
       virtual void setupShading(string shaderName);
       virtual void setupTexturing(string texName, Actor* a=NULL, GLenum texChannel=GL_TEXTURE0);


       virtual void transformTextureMatrix(Actor* a);
       virtual void transformActorMatrix(Actor* a);

       void pick(int x, int y);                    //get the 3D coordinates from the mouse and also get the actor we're pointing at

       static GLuint LoadTextureRAW( const char * filename, int size, int wrap );
       bool LoadTextureTGA( string filename, bool wrap, bool bAlpha, string texID );
       bool createEmptyTexture( string texID, GLuint colorFormat, GLuint type, int width, int height);

       bool copyMemoryToTexture(void* originBuffer,string texName, float width,float height,bool bHighZRes=false);

       bool loadShader(string vertexShaderFileName, string fragmentShaderFileName, string shaderProgramName);

       void printShaderInfoLog(GLuint obj);
       void printProgramInfoLog(GLuint obj);

       bool checkOpenGLError(string preText=" ", bool bPrint=true);

    //has to be static from physics!
      // static void handleCollisionBetween(void * data, dGeomID o0, dGeomID o1);

      };


#endif
