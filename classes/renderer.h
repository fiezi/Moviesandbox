#ifndef H_RENDERER
#define H_RENDERER

#include "content.h"
#include "brush.h"
#include "layer.h"
#include "brush.h"
#include "particleSystem.h"
#include "basicButton.h"
#include "node.h"
#include "action.h"
#include "textfile.h"
#include "flexGrid.h"

#include "msbLight.h"


#define BUFSIZE 512



using namespace std;

class Input;
class MeshData;
class ColladaLoader;
class SpriteMeshLoader;
class SpriteMeshLoaderXML;

class Renderer{

public:


        //*****************************************
        //Template for creating Actor IDs
        //*****************************************
        template<typename myType>
        void createActorID(myType *ref ){
            actorID *act;
            act=new actorID;
            //Actor
            act->actorReference=ref;
            act->actorType=&(typeid(*ref));
            act->actorSize=sizeof(*ref);
            actorInfo[typeid(*ref).name()]=*act;
            std::cout << "created a reference for " << typeid(*ref).name() << "\n";
            std::cout << "created a reference for " << act->actorType->name() << "\n";
        }

        //*****************************************
        //Template for creating an Actor
        //*****************************************
        template <typename myType>
        void addActor (myType *a){

            a = new myType;
            a->renderLayer=currentLayer;
            actorList.push_back(a);
            char newName[8];
            sprintf(newName,"%d",(int)actorList.size()-1);
            a->name+=newName;
            layerList[currentLayer]->actorList.push_back(a);
        }

        //*****************************************
        //Template for creating an Actor
        //*****************************************
        template <typename myType>
        void addButton (myType *b){
            Actor *ac;
            ac = new myType;
            BasicButton *basePtr;
            basePtr = dynamic_cast<BasicButton*>(ac);

            buttonList.push_back(basePtr);

            //add to save list if we are a saveable button!
            if (basePtr->bPermanent)
                saveableButtonList.push_back(basePtr);
        }

        //*****************************************
        //Template for creating a Node
        // Nodes are also represented in the buttonList!
        //*****************************************
        template <typename myType>
        void addNode (myType *n){
            Actor *ac;
            ac = new myType;
            Node *basePtr;
            basePtr = (Node*)ac;
            nodeList.push_back(basePtr);
            buttonList.push_back(basePtr);
        }

        //*****************************************
        //Template for creating an Action
        // Actions are also represented in the buttonList!
        //*****************************************
        template <typename myType>
        void addAction (myType *b){
            Actor *ac;
            ac = new myType;
            Action *basePtr;
            basePtr = dynamic_cast<Action*>(ac);
            buttonList.push_back(basePtr);

        }

        //verion debug

       static Renderer* rendererInstance;
       Input*           input;

       map <string, actorID> actorInfo;    //for RTTY - all types of actors this program has
                                           //and type/size information for them

       GLuint selectBuf[BUFSIZE];          //for picking

       std::string  startSceneFilename;    //basic scene
       vector<std::string>  library;       //all libraries

       int currentLayer;                    //currently selected renderLayer
       string currentShader;                //currently bound shader

       //lists
       vector <Actor*> actorList;          //all created actors go here
       vector <Actor*> helperList;         //brush and grid go here
       vector <MsbLight*> lightList;         //brush and grid go here

       vector <BasicButton*> buttonList;        //all created buttons go here
       vector <BasicButton*> saveableButtonList;    //all saveable Buttons go here
       vector <Node*> nodeList;            //all created nodes go here
       vector <Layer*> layerList;              //all Layers
       map <string, Action*> actionList;    //all actions
       map <string, externalInputData*> externalInputList;    //all external programs for data input

       //map <string, ObjFile> meshList;     //old Mesh List
       map <string, MeshData*> vboList;  //new Vertex Buffer Object List
       map <string, textureObject*> textureList;   //all loaded textures go here
       map <string, shaderObject*> shaderList;    //all loaded shaders go here

       string backgroundTex;
       Vector4f backgroundColor;
       string lastShader;

       Actor* grid;                        //direct pointer to Grid
       Brush* brush;                       //direct pointer to Brush

       bool bRenderStereo,                  //render stereoscpic into Framebuffers
            bDrawLighting,                  //include Lights and Shadows in Rendering
            bDrawNodes,                     //draw nodes
            bDrawMenu,                      //show menu in 3D renderer
            bUseBlending,
            bMultisample,                   //Multisample FBOs
            bSSAO,                          //draw screen-space ambient occlusion
            bDrawColor;                     //draw basic Color image (would be lighting only if false)

       int numSamples;                      //number of multisamples

       //Physics using ODE
       dWorldID physicsWorld;              // our physics world
       dSpaceID collisionSpace;            // using ODE collision
       dJointGroupID jointGroup;           // a joint group
       dGeomID groundPlane;                // our ground plane
       bool bUpdatePhysics;                // call physicsUpdate or not
       bool bDebug;                        //Debug mode, will check for OpenGL Errors

       //al and Screen stuff
       bool bFullscreen;
       int screenX,screenY;                //rendered screen width, height - maybe rename..
       int windowX,windowY;                //window width and height
       float nearClip, farClip;            //clipping planes

       float frustumTop,frustumBottom;      //viewFrustum for 3D stuff
       float eyeDistance;                   //eyeDistance for 3D rendering
       float bkgOffset;                     //eyeDistance for 3D rendering

       int frames;                         //amount of frames rendered
       double currentTime,                 //time since program start,
              deltaTime,                   //time since last frame
              physicsTime;                 //time for physics step (sometimes deltaTime is too small to advance physics...)

       //Control stuff
       float mouseSensitivity;             //mouse sensitivity and general turn speed
       float moveSpeed;                    //general move speed

       float fov;                          //field of view

       Vector3f lightLoc;                  //light Location
       Vector3f ambient;                   //ambient Light

       Content *content;                   //the list of things we create at program start
       ColladaLoader* colladaLoader;       //helper object for loading meshes and animations
       SpriteMeshLoader* spriteMeshLoader; //helper Object for loading sprite meshes from Base64
       SpriteMeshLoaderXML* spriteMeshLoaderXML; //helper Object for loading old sprite meshes from XML

        //picking
        GLuint pickTexture;

        //Frame Buffer Objects for light and shadow rendering

        GLuint lighting_tx;                 // the lightTexture we read from
        GLuint lighting_fb;                 // the lightFBO we bind

        GLuint shadow_tx;                   // the shadowTexture we read from
        GLuint shadow_fb;                   // the shadowFBO we bind

        int shadow_size;
        //Frame Buffer Object for Depth Rendering
        GLuint depth_tx;
        GLuint depth_fb;
        int depth_size;

        int scene_size;

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

		GLenum drawBuffers[3];

        BasicButton* leftEyeFBO;                 //2D planes for left and right eye!
        BasicButton* rightEyeFBO;
        BasicButton* postOverlay;

        //Matrices
        Matrix4f projectionMatrix;
        Matrix4f inverseProjectionMatrix;
        Matrix4f cameraMatrix;
        Matrix4f inverseCameraMatrix;

        Matrix4f cameraProjectionMatrix;
        Matrix4f cameraViewMatrix;
        Matrix4f camViewProjMatrix;

        Matrix4f lightViewMatrix;
        Matrix4f lightProjectionMatrix;
        Matrix4f lightViewProjMatrix;



       Renderer();
       virtual ~Renderer();

       //singleton-ish
       static Renderer* getInstance();

       //***************************************
       //this is stuff we can update easily
       //***************************************
       virtual void fillGlobalLists();                  //for RTTY - generate actorInfo
       virtual void initWindow(int x, int y, string windowName);  //
       virtual void reDrawScreen(int w, int h);

#ifdef TARGET_WIN32
       static bool ModifyWindowStyle(HWND hWnd, DWORD dwAdd, DWORD dwRemove, BOOL bEx);
#endif
       virtual void setup();                      //first thing called when program starts
       virtual void physicsSetup();                      //setup ODE

       virtual void loadPreferences();

       virtual void createFBO(GLuint* fbObject, GLuint* fbTexture, GLuint* fbDepth, int fbSize, bool bDepth, string name);
       virtual void checkFBOStatus();

       virtual int readSharedMemory();

       virtual void update();
       virtual void physicsUpdate();

       virtual void addLayer(string layerName);
       virtual void addGrid();
       virtual void addBrush();

       virtual void draw();

       virtual void drawBackground();

       virtual void drawSceneTexture();

       virtual void draw3D(Layer* currentLayer);
       virtual void draw2D();

       virtual void drawActor(Actor* a);
       virtual void drawButton(BasicButton* b);

       virtual void draw3DOverlay();

       virtual void drawShadows(MsbLight* myLight);
       virtual void drawDeferredLighting(Layer* layer);

       virtual void drawBone(float width, float height, float depth);
       virtual void drawCube(float scale, float cubeSize);
       virtual void drawPlane(float x1,float  y1,float  x2,float  y2, Vector4f color = Vector4f(1.0,1.0,1.0,1.0), bool bCentered = false);
       virtual void drawLine(Vector3f start,Vector3f end, Vector4f startColor=Vector4f(0,0,0,1), Vector4f endColor=Vector4f(0,0,0,1));
       virtual void drawPatch(float width, float height, float resolution);
       virtual void drawOrientation(Actor* a);
       virtual void drawSprite();

       virtual void drawParticles (Actor* a);
       virtual void drawColladaMesh (Actor* a);

       virtual void setupCamera(bool bCalculateMatrices);
       virtual void setupShading(string shaderName);
       virtual void setupTexturing(string texName, Actor* a=NULL, GLenum texChannel=GL_TEXTURE0);


       virtual void transformTextureMatrix(Actor* a);
       virtual void transformActorMatrix(Actor* a);

       void pick(int x, int y);                    //get the 3D coordinates from the mouse and also get the actor we're pointing at

       GLuint createNewRenderTarget();
       static GLuint LoadTextureRAW( const char * filename, int size, int wrap );
       bool LoadTextureTGA( string filename, bool wrap, bool bAlpha, string texID );
       bool createEmptyTexture( string texID, GLuint colorFormat, GLuint type, int width, int height);

       bool loadShader(string vertexShaderFileName, string fragmentShaderFileName, string shaderProgramName);

       void printShaderInfoLog(GLuint obj);
       void printProgramInfoLog(GLuint obj);

       bool checkOpenGLError(string preText=" ", bool bPrint=true);

    //has to be static from physics!
       static void handleCollisionBetween(void * data, dGeomID o0, dGeomID o1);

      };


#endif
