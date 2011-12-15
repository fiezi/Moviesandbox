#ifndef H_ACTOR
#define H_ACTOR

#include "msbObject.h"

#define DRAW_PLANE          0
#define DRAW_MESH           1
#define DRAW_TEA            2
#define DRAW_CUBE           3
#define DRAW_PARTICLES      4
#define DRAW_PARTICLES_LINE 5
#define DRAW_PARTICLES_PATH 6
#define DRAW_SPRITE         7
#define DRAW_VBOMESH        8
#define DRAW_SPECIAL        9
#define DRAW_POINTPATCH     10
#define DRAW_KINECT         11


#define DRAW_NULL           100


//*****************************************
//Template for assigning memberIDs
//*****************************************


class Control;
class Input;
class Renderer;
class SceneData;
class InterpolationHelper;
class Node;
class Action;

class Actor: public MsbObject{

        /***********************************************************************************

        Variables

        ************************************************************************************/

public:

       int renderLayer;                             //layer in which this Actor is being rendered

       Vector3f location,                           //space properties
                rotation,
                scale,
                orientation,                        //orientation of actor in world space
                xAxis,                              //three axis that make up the rotation of the actor
                yAxis,
                zAxis;

       // Animation blending
       float transitionTime;                        //time to interpolate between different movers
       vector<float> currentTransitions;            //current Time in transition
       int numberOfMovers;                          //number of Movers in last frame

       vector<Vector3f> moverLocations;             //buffers for different movers
       vector<Vector3f> moverRotations;
       vector<Matrix4f> moverMatrices;

       float collisionCubeSize;                     //the size of the pickable cube - outdated but useful sometime down the line i guess

       float particleScale;                         //size multiplier for particle Mesh
       float particleAngleScale;                    //angle size attenuation multiplier for particle Mesh (to fill holes)


       //bounding box related
       Vector3f    lowerLeftBack,                   //point is relative to object position
                   upperRightFront,
                   center;                          //center of bounding box!

       Vector3f pivotLocation;                      //for rotation around a different point - is relative to location!!!

       Matrix4f originalMatrix;                     //original transformation - makes transformmatrix relative to this one - used for bones!
       Matrix4f transformMatrix;                    //our individual transform, without base transformations
       Matrix4f scaleMatrix;                        //our scaling


       Matrix4f initialTransform;                   //for resetting after scene finished playing
       Vector3f initialLocation;                    //mainly used for buttons!
       Vector3f initialRotation;

       Actor    *base;                              //for forward kinematics and hirarchical animation
       Matrix4f  baseMatrix;                        //stores the final actor matrix (to be used as base for the next child)

       std::string      groupID;                            //which group this actor belongs to.

       Node     *baseNode;                          //the node controlling this actor

       std::string sceneShaderID;                   //shader identifier

       std::string vboMeshID;                       //new COLLADA mesh identifier

       std::string textureID;                         //texture identifier for texture instancing

       int drawType;                                //which way to draw this actor: e.g. DRAW_CUBE, DRAW_MESH, DRAW_SPRITE, etc...

       double birth;                                //time when this actor was created

       bool    bInit;                               //is false before loading

       bool bHidden;                                //is rendered
       bool bPickable;                              //can be clicked on (mouse)
       bool bRemoveable;                            //cannot be removed...
       bool bSelected;                              //is in sceneData->selectedActors
       bool bHighlight;                              //is in sceneData->selectedActors

       //Render options
       bool bTextured,                              //uses Texture Map in TextureUnit 0
            bShadowTextured,                                //uses ShadowMap in TextureUnit 1
            bZTest,                                 //checks Z-Buffer before drawing
            bZWrite,                                //writes to Depth Buffer
            bUseShader,                             //uses Shader for drawing
            bComputeLight,                            //lighting disabled
            bDrawOrientation,                       //draws RGB Lines for XYZ Orientation
            bDebug;                                 //prints out debug information

       bool bLight;                                 //counts as light

       GLuint   blendModeOne;                       //first Blend mode
       GLuint   blendModeTwo;                       //second Blend mode

       Control *controller;                         //somewhat undefined, takes control when scene is playing
       vector<InterpolationHelper*> movers;                  //moves the actor - will be destroyed when keyframe reached

       //for radial menu
       std::vector <std::string> menuType;          //menu list for actor (when mouse is clicked on actor
       std::vector <std::string> menuProp;
       std::vector <std::string> menuName;
       std::vector <std::string> menuIcon;
       //end radial menu

        // timer for texture animation
        float   textTimer;

        // texture properties
        Vector3f    texScale,
                    texRotation,
                    texTranslation;

        //for actor loading
        int actorOffset;
        int nodeOffset;

        //for actor picking
        float objectID;


        /***********************************************************************************

        Functions

        ************************************************************************************/

       Actor();
       virtual ~Actor();

       //setup functions
       virtual void registerProperties();           //fill property map with all user-changeable variables as string properties

       virtual void postLoad();                     //gets called immediately after all variables have been loaded, but before setup
       virtual void setup();
       virtual void trigger(MsbObject* other);         //general trigger function

       //runtime looping functions
       virtual void update(double deltaTime);       //handles interpolation
       virtual void updateShaders();                // uniform updates
       virtual void draw();                         // sets up rendering flags and transformations

       //callback for highlighting
       virtual void mouseOver();

       //drawType functions - the actual glBegin() stuff is in here
       virtual void drawSprite();
       virtual void drawPlane();
       virtual void drawParticles();
       virtual void drawTeapot();
       virtual void drawCube();

       //get back to initial state when scene started
       virtual void reset();

       virtual void start();                            //gets called from controller when running scene
       virtual void stop();                             //gets called from controller when running scene

       virtual void setBase(Actor* newBase);            //set Base and keep actor's position on screen

       //transformation functions - please always call!
       virtual void setLocation(Vector3f loc);          //set translation
       virtual void addLocation(Vector3f loc);          //add to current translation
       virtual void setAbsoluteLocation(Vector3f loc);          //set translation
       virtual void setRotation(Vector3f rot);          //set rotation
       virtual void setRotation(Vector3f xA, Vector3f yA, Vector3f zA); //set rotation from 3 axis
       virtual void addRotation(float amount,Vector3f axis);          //add to rotation - used for mouse rotation when bUseTransformmatrix
       virtual void setScale(Vector3f newScale);

       virtual void getAxis(Vector3f* xA, Vector3f* yA, Vector3f* zA, Matrix4f myMatrix);
       virtual void setRotationFromAxis( Vector3f xA, Vector3f yA, Vector3f zA);
       virtual Matrix4f calcMatrix(Actor* myActor);     //recursive function to determine absolute transformMatrix
       virtual Vector3f calcEuler(Matrix4f myMatrix);   //Rotation Matrix to Euler Angles
       virtual void matrixToVectors();              //generates location vector, axis and rotation in euler angles

        virtual void setTextureID(string texID);            //use this to set textureID to prevent texture load errors and Crashes to Desktop!
        virtual void setMeshID(string vboID);               //use this to set vboMeshID to prevent mesh load errors and Crashes to Desktop!

       //general saving and loading - repeated from MsbObject!
       virtual TiXmlElement* save(TiXmlElement *root);
       virtual TiXmlElement* saveAsPrefab(TiXmlElement *root);  //saves all properties as defined in registerProperties
       virtual void load(TiXmlElement *myInfo);

       //these two are slightly altered for prefabs!
       virtual Actor* readActor(char* cValue);                         //identical to readInt, but parses the actor* tag in the string!
       virtual Node* readNode(char* cValue);                          //identical to readInt, but parses the node* tag in the string!

       virtual void removeReferences();                 //removes the actor from actorList, buttonList, other actors' properties, other buttons' properties and so on
       virtual void remove();                           //calls removeReferences, then frees memory.

       bool isA(string className);                      //check against specific classname

       bool createNewActor(string actorIDName);         //runtime Actor spawning through a string

       Actor* spawn(string actorIDName);

       virtual void create();                           //runtime Actor creation
};



#endif
